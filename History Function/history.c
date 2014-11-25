
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"
#include "list.h"

#define BUF_MAX  (81)
#define COM_MAX  (10)

typedef struct {
  char name[BUF_MAX];
} command_info;

List_t history;
int command_num;

/* Init */
int
History_Init()
{
	int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
	
	/* Initialize history data structures. */
	if (List_init( &history )) {
		command_num=0;
		
	} else {
		printf ("Unable to initialize history.\n");
		error_code = 1;
	}

	//printf ("finish History_Init().\n");
	return error_code;
}

/* View */
int
History_View()
{
	int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
	int cnt=0;
	command_info   *context=NULL;
	command_info   *mydata;

	while(List_next_node( &history, (void*) &context, (void*) &mydata ) && (mydata != NULL))
	{
		printf("%2d: %s", cnt, mydata->name);
		cnt++;
	}
	
	//printf ("finish History_View().\n");
	return error_code;
}

/* Choose */
int
History_Choose()
{
	int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
	int	   cnt=0;
	char   inputbuffer[BUF_MAX];
	int index=0;
	
	error_code = History_View();
	if(error_code != 0)
	{
		printf("an error occurred in history_view().\n");
		return error_code;
	}

	printf("\nPlease choose a number of command you wish to operate: ");
	if (fgets( inputbuffer, BUF_MAX-1, stdin )) {
		
		index = atoi(inputbuffer);
		printf("command_num: %d\n", command_num);
		
		//param check
		if(command_num < index || index < 0)
		{
			printf("Chosen number is invalid: %d\n", index);
			error_code = 1;
			return error_code;
		}

	} else {
		printf ("Nothing read in.\n");
    }//error fgets()
	
	command_info   *context=NULL;
	command_info   *mydata;
	command_info   *command;

	while(List_next_node( &history, (void*) &context, (void*) &mydata ) && (mydata != NULL))
	{
		if (cnt == index)
		{
			command = (command_info *) malloc( sizeof( command_info ) );
			if (command != NULL) {			
				strcpy(command->name, mydata->name);  
				List_remove_in_context( &history, (void*) &context, (void*) &mydata );
				List_add_head ( &history, (void*) command );
			}
			break;
		}
		cnt++;
	}
	
	//printf ("finish History_Choose().\n");
	return error_code;
}

/* Add */
int
History_Add(char* inputbuffer)
{
  int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
  //char   inputbuffer[BUF_MAX];
  command_info  *command;

	/* Put the parameters into a COMMAND and store it in the list of command_info. */
	  command = (command_info *) malloc( sizeof( command_info ) );
	  if (command != NULL) {
	    strncpy( command->name, inputbuffer, BUF_MAX-1 );
	    command->name[BUF_MAX] = '\0'; /* Make sure that it is null-terminated. */
	    printf ("Read and stored command, %s\n", command->name);

		//[ADD] functionality
		//if command history contains maximum number of commands, remove the last command
		if (command_num == COM_MAX)
		{
			int cnt=0;
			command_info   *context=NULL;
			command_info   *mydata;

			//search for the last command
			while(List_next_node( &history, (void*) &context, (void*) &mydata ) && (mydata != NULL))
			{
				cnt++;
				//if cnt reaches COM_MAX (=if found the last command), remove the last command
				if(cnt == COM_MAX)
				{
					List_remove_in_context(&history, (void*) &context, (void*) &mydata);
				}
			}
		}	
		
		//add new command to the head of the history list
	    if (List_add_head( &history, (void *)command ) == 0) {
	      printf ("Error in inserting the command into the list.\n");
	    } else {
			//increment # of command
			if(command_num < COM_MAX)
			{
				command_num++;
			}
		}
		
	  } else {
	    printf( "Unable to allocate enough memory.\n");
	  }
	  
	//printf ("finish History_Add().\n");
	return error_code;
}

