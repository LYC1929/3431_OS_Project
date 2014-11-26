#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFSIZE 100

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

int print_prompt(void)
{
    printf("\n[GS Shell] ^_^ Enter Command: ");
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
static char *do_tab()
{
    char *line = NULL;
    line = readline("\n[GS Shell] ^_^ Enter Command: ");
    
    return line;
}


char* cmd[] = {"cd", "clear", "help", "history", "ls", "mkdir", "pwd", "quit", "rm", "rmdir", "stat", "time", "vim"};

static char** gs_completion(const char * text, int start, int end)
{
    char **matches;
    
    matches = (char **)NULL;
    
    if (start == 0)
        matches = rl_completion_matches((char*)text, &gs_generator);
    else
        rl_bind_key('\t', rl_insert);
    
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


// main
int main(int argc, char** argv)
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
		// enable auto-complete
        rl_bind_key('\t', rl_complete);
		char buf[BUFSIZE] = {0};

		line = do_tab();
		len = 0;
	    memcpy(buf, line , BUFSIZE);
		len = strlen(buf);
		if(len >= BUFSIZE)
        {
			printf("command is too long\n");
			break;
		}
    
		if(strcmp("quit", buf) == 0)
			break;
        else if(strcmp("quit ", buf) == 0)
            break;

        if(0 == do_history(buf));
        else
        {
		    do_list_cmd(buf,len + 1);
        }
        
        if (buf[0] != 0)
            add_history(buf);
	}
    
    return 0;
}
