#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFSIZE 100

char* myptr;
char* mylim;

char lastdir[100];
char history_dir[10][BUFSIZE];
int history_count = 0;

void init_lastdir()
{
    getcwd(lastdir, 99);
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
            printf("%s\n",history_dir[i]);
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
//		execv(argv[0],argv);
		system(argv);
//		printf("PID %d\n",(int)getpid);
		return 0;
	}
	if((pid = fork()) == 0) {   // child
		// reset the signal INT handle to default
		int redirect = 0;
		signal(SIGINT, SIG_DFL);

		if(redirect != 1 && prefd) {    // has a pre pipe, redirect stdin
            close(prefd[1]);
			if(prefd[0] != STDIN_FILENO) {
//	            fprintf(stderr, "redirect stdin\n");
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
//		printf("PID %d\n",(int)getpid);
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
//    printf("%s",argbuf);
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
                do_simple_cmd(i-j,argbuf+j,prefd,postfd);
            else
                do_simple_cmd(i-j,argbuf+j,0,postfd);
                    argbuf[i] = p;
            prepipe = 1;
            prefd[0] = postfd[0];
            prefd[1] = postfd[1];
                j = ++i;
        } else
            i++;
    }
    
    if(prepipe)
        do_simple_cmd(i-j,argbuf+j,prefd,0);
    else
    {
        do_simple_cmd(i-j,argbuf+j,0,0);
    }
}
