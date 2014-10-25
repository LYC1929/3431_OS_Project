CC = gcc
CFLAGS = -g -o
LDFLAGS = -lncurses
EXECS = exec
FILE = *.c

all: compile clean

compile: $(FILE)
	$(CC) $(CFLAGS) $(EXECS) $(FILE) $(LDFLAGS)

clean: 
	-rm -rf *.dSYM

spotless: 
	-rm $(EXECS)
