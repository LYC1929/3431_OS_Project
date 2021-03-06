#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFSIZE 100
#define MAX_PROMPT 1024

const int max_name_len = 256;
const int max_path_len = 1024;

struct passwd *pwd;

char lastdir[100];
char history_dir[10][BUFSIZE];
int history_count = 0;

static char** gs_completion(const char*, int ,int);
char* gs_generator(const char*,int);
char * dupstr (char*);
void *xmalloc (int);

void init_lastdir()
{
    getcwd(lastdir, 99);
}


// draw the shell name
static void drawHs()
{
    printf("\n");
    printf("        __________    __________     \n");
    printf("       |  ________|  |  ________|    \n");
    printf("       | |           | |             \n");
    printf("       | |     ___   | |________     \n");
    printf("       | |    |_  |  |_________ |    \n");
    printf("       | |      | |           | |    \n");
    printf("       | |______| |   ________| |    \n");
    printf("       |__________|  |__________|    \n");
    
    printf("\n");
    printf("        __________    _        _    __________    _            _            \n");
    printf("       |  ________|  | |      | |  |  ________|  | |          | |           \n");
    printf("       | |           | |      | |  | |           | |          | |           \n");
    printf("       | |________   | |______| |  | |________   | |          | |           \n");
    printf("       |________  |  |  ______  |  |  ________|  | |          | |           \n");
    printf("                | |  | |      | |  | |           | |          | |           \n");
    printf("        ________| |  | |      | |  | |________   | |_______   | |_______    \n");
    printf("       |__________|  |_|      |_|  |__________|  |_________|  |_________|   \n");
    
    printf("\n================================================================================\n");
    printf("================================================================================\n");
};


// print prompt
static char *print_prompt()
{
    char *prompt1[MAX_PROMPT];
    char *prompt2[MAX_PROMPT];
    char *prompt3[MAX_PROMPT];
    char *prompt[MAX_PROMPT];
    char *line = NULL;
    char hostname[max_name_len];
    char pathname[max_path_len];
    char length;
    
    pwd = getpwuid(getuid());
    getcwd(pathname,max_path_len);
    
    if(gethostname(hostname,max_name_len)==0)
        sprintf(prompt1, "\n[GS Shell]%s@%s:", pwd->pw_name, hostname);
    else
        sprintf(prompt1, "\n[GS Shell]%s@unknown:", pwd->pw_name);
    
    if(strlen(pathname) < strlen(pwd->pw_dir) || strncmp(pathname, pwd->pw_dir, strlen(pwd->pw_dir)) != 0)
        sprintf(prompt2, "%s", pathname);
    else
        sprintf(prompt2, "~%s", pathname+strlen(pwd->pw_dir));
    
    if(geteuid()==0)
        sprintf(prompt3,"# ");
    else
        sprintf(prompt3,"$ ");
    
    prompt[0]=0;
    strcat(prompt, prompt1);
    strcat(prompt, prompt2);
    strcat(prompt, prompt3);
    
    line = readline(prompt);
    
    return line;
}


// pipe
static int do_simple_cmd(int argc, char** argv, int prefd[], int postfd[])
{
    int pid;
    int status;
    char buffer[100];
    if(argc == 0)
        return 0;
    if(prefd == 0 && postfd == 0) {
		system(argv);
		return 0;
	}
	if((pid = fork()) == 0) {   // child
		// reset the signal INT handle to default
		int redirect = 0;
		signal(SIGINT, SIG_DFL);

		if(redirect != 1 && prefd) {    // has a pre pipe, redirect stdin
            close(prefd[1]);
			if(prefd[0] != STDIN_FILENO) {
				dup2(prefd[0], STDIN_FILENO);
				close(prefd[0]);
			}
		}
		if(redirect != 2 && postfd) {   // has a post pipe, redirect stdout
			close(postfd[0]);
			if(postfd[1] != STDOUT_FILENO) {
				dup2(postfd[1], STDOUT_FILENO);
				close(postfd[1]);
			}
		}
		system(argv);
	}
    
	waitpid(pid, &status, 0);
	if(postfd) {    // no
		close(postfd[1]);   // must close this fd here
	}
	return 0;
}

static void do_list_cmd(const char *buf,const int len)
{
    int i = 0;
    int j = 0;
    int prepipe = 0;
    char* p;
    char argbuf[BUFSIZE] = {0};
    int prefd[2],postfd[2];
    pid_t pid;

    memcpy(argbuf,buf,len);

    while(argbuf[i])
    {
        if(strcmp(&argbuf[i], "|") == 0) {
            p = (char *)argbuf[i];
            argbuf[i] = 0;
            if(pipe(postfd) < 0)
            {
                printf("Create pipe failed");
                exit(-1);
            }
            if(prepipe)
                do_simple_cmd(i-j, argbuf+j, prefd, postfd);
            else
                do_simple_cmd(i-j, argbuf+j, 0, postfd);
                    argbuf[i] = p;
            prepipe = 1;
            prefd[0] = postfd[0];
            prefd[1] = postfd[1];
                j = ++i;
        } else
            i++;
    }
    
    if(prepipe)
        do_simple_cmd(i-j, argbuf+j, prefd, 0);
    else
    {
        do_simple_cmd(i-j, argbuf+j, 0, 0);
    }
}


// tab completion
char* cmd[] = {"cd", "clear", "help", "history", "ls", "mkdir", "pwd", "quit", "rm", "rmdir", "stat", "time", "vim"};

static char** gs_completion(const char * text, int start, int end)
{
    char **matches;
    
    matches = (char **)NULL;
    
    if (start == 0)
        matches = rl_completion_matches((char*)text, &gs_generator);
    else
        rl_bind_key('\t', rl_insert);   // disable the default TAB behavior
    
    return (matches);
    
}

char* gs_generator(const char* text, int state)
{
    static int list_index, len;
    char *name;
    
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }
    
    while (name = cmd[list_index]) {
        list_index++;
        
        if (strncmp (name, text, len) == 0)
            return (dupstr(name));
    }
    
    /* If no names matched, then return NULL. */
    return ((char *)NULL);
    
}

char * dupstr (char* s) {
    char *r;
    
    r = (char*) xmalloc ((strlen (s) + 1));
    strcpy (r, s);
    return (r);
}

void * xmalloc (int size)
{
    void *buf;
    
    buf = malloc (size);
    if (!buf) {
        fprintf(stderr, "Error: Out of memory. Exiting.'n");
        exit(1);
    }
    
    return buf;
}


// history
static int do_history(char *buf)
{
    int i;
    if(history_count >= 10)
    {
        history_count = 0;
    }
    
    memset(&history_dir[history_count][BUFSIZE], 0, BUFSIZE);
    memcpy(&history_dir[history_count][BUFSIZE], buf,  BUFSIZE);
    history_count++;
    
    if(strcmp("history", buf) == 0 || strcmp("history ", buf) == 0)
    {
        for( i = 1; i < 10; i++)
        {
            if(history_dir[i][0] == '\0')
            {
                break;
            }
            printf("%s\n", history_dir[i]);
        }
    }
    else
    {
        return -1;
    }
    
    return 0;
}


// main
int main(int argc, char** argv, char** envp)
{
    drawHs();
    
    int len;
    char ch;
    char *line = NULL;
    
    signal(SIGINT, SIG_IGN);

    init_lastdir();
    
   	rl_attempted_completion_function = gs_completion;
    
	while(1)
	{
		char buf[BUFSIZE] = {0};

		line = print_prompt();
        
		len = 0;
	    memcpy(buf, line , BUFSIZE);
        
        for (int i=0; i<strlen(buf); i++)
            buf[i]=tolower(buf[i]);
        
		len = strlen(buf);
		if(len >= BUFSIZE)
        {
			printf("command is too long\n");
			break;
		}

        // enable auto-complete
        rl_bind_key('\t', rl_complete);
        
        if(strcmp("quit", buf) == 0 || strcmp("quit ", buf) == 0)
        {
            printf("logout\n\n");
            break;
        }

        if(0 == do_history(buf));
        else
        {
		    do_list_cmd(buf,len + 1);
        }
        
        if (*buf)
            add_history(buf);
	}
    
    return 0;
}
