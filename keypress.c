#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main()
{

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
		if (ch == '\t')
			printw("\nYou entered a tab\n");

		else if (ch == '\n'){
			printw("\n$ ");
		}
		else if (ch == 'q')
			break;
		else
			printw("%c", ch);
		refresh();	
	}
	endwin();
	return 0;
}

