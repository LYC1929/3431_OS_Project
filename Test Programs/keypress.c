#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main()
{
	int count=0;
	char *str="";
	initscr();
	raw();
	keypad(stdscr, TRUE);
	echo();

	printw("Enter something: \n");
	printw("$ ");
	refresh();
	while(1)
	{
		
		char ch=getc(stdin);
		if (ch == '\t')		//if detect a tab
			printw("\nYou entered a tab\n");

		else if (ch == '\r'){	//if detect an enter
			printw("\n$ ");
		}
		else if (ch == 'q')	//if want to quit
			break;
		else if (ch == 127){	//if detect a backspace
			/* deal with delete */
		}
		else
			printw("%c", ch);
		refresh();	
	}
	endwin();
	return 0;
}

