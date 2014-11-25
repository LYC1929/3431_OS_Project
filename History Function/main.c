#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

#define BUF_MAX  (81)

/* This code is to test history, and needed to be merge with "pipe" and "tab"*/
int
main( int argc, char **argv )
{
  int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
  char   inputbuffer[BUF_MAX];
  char   inputbuffercom[BUF_MAX];
  
  error_code = History_Init();
  if(error_code != 0)
  {
	printf("an error occurred in history_init().\n");
	return error_code;
  }

  while (1) {
	  printf("\n"); 
	  printf("  1: View your command history.\n");
	  printf("  2: Add a command.\n");
	  printf("  3: Choose a command.\n");  
	  printf("Please choose a number you want to operate: ");
  
      if (fgets( inputbuffer, BUF_MAX-1, stdin )) {
		
		//View
		if(inputbuffer[0] == '1') {
			error_code = History_View();
			if(error_code != 0)
			{
				printf("an error occurred in history_view().\n");
				return error_code;
			}
		}//end View
		
		//Add
		else if(inputbuffer[0] == '2'){

			printf("Please type command: ");
			if (fgets( inputbuffercom, BUF_MAX-1, stdin )) {

			  error_code = History_Add(inputbuffercom);
			  if(error_code != 0)
			  {
				printf("an error occurred in history function().\n");
				return error_code;
			  }
			
			} else {
				printf ("Nothing read in.\n");
			}
		}//end Add

		//Choose
		else if(inputbuffer[0] == '3'){			
			error_code = History_Choose();			
			if(error_code != 0)
			{
				printf("an error occurred in history function().\n");
				return error_code;
			}
		}//end Choose
		
		//Error
		else
		{
			printf("Chosen number is invalid: %c\n", inputbuffer[0]);
		}//end Error
		
    } else {
		printf ("Nothing read in.\n");
    }
  }//end while

  return error_code;
}

