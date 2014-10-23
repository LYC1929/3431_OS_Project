CC = gcc
CFLAGS = -g -o
EXECS = exec
FILE = *.c

all: compile clean

compile: $(FILE)
	$(CC) $(CFLAGS) $(EXECS) $(FILE)

clean: 
	-rm -rf *.dSYM

spotless: 
	-rm $(EXECS)
