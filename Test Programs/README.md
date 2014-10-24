Test Program Folder
===

This folder contains test programs for the project, it currently contains only one program: `keypress.c`, which is a test program for listening to the tab key (`'\t'`).

***
 
Test Programs
-------


- `keypress.c`: The program is meant to be a test program for tab-completion. It uses a library called `ncurses`, when it is executed, it changes the stdin to raw mode and listen to every key press, if a tab key is pressed, it will print out a line of sentence. When compile the file, we need to add `-lncurses` as the DLFLAGS.

***

Known Issues
-------

- `keypress.c`: The program is not able to produce a `'\n'`.
