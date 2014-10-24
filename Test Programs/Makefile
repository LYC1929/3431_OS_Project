CC = gcc
CFLAGS = -g -Wall -o
EXECS = exec
DLFLAGS = -lncurses
FILES = *.c

all: compile clean

compile: $(FILES)
	$(CC) $(CFLAGS) $(EXECS) $(FILE) $(LDFLAGS)

clean:
	-rm -rf *.dSYM

spotless:
	-rm $(EXECS)
