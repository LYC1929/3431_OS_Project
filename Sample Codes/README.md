Sample Code Folder
===

This folder contains the sample codes for various libraries, function, etc. It now only have codes for pipes, which can be later used for implementing pipe-like commands in the shell we are to implement.

***

Sample codes
------

- `pipe1.c`: The parent process closes the pipe after the child process is terminated.

- `pipe2.c`: The parent process closes the pipe before the child process is terminated and after the message is sent.

- `popen1.c`: Execute `ls` command in another process

- `popen2.c`: Execute `sort` command in another process
	1. create a file with words
	2. open a pipe and sort the words in the file
	3. get rid of the file now that it's done

***

Know Issues
------

TBA

