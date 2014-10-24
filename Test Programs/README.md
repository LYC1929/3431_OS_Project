Test Program Folder
===

This folder contains test programs for the project, it currently contains two programs: `keypress.c` and `Database_Integration.c`. The details are below.

***
 
Test Programs
-------


- `keypress.c`: The program is meant to be a test program for tab-completion. It uses a library called `ncurses`, when it is executed, it changes the stdin to raw mode and listen to every key press, if a tab key is pressed, it will print out a line of sentence. When compile the file, we need to add `-lncurses` as the DLFLAGS.

- `Database_Integration.c`: The program will promote users to input information about a database and then connects to the database and perform ONE (for now) sql query. When compile the file, **it has to be under an UNIX/LINUX environment** (maybe also Widnows) using '\`mysql_config --cflags --libs\`' as DLFLAGS

***

Known Issues
-------

- `keypress.c`: The program is not able to produce a `'\n'`.
