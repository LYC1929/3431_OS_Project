CC = gcc
CFLAGS = -g -Wall -o
EXECS1 = execKey
EXECS2 = execDB
DLFLAGS = -lncurses
DLFLAGS2 = `mysql_config --cflags --libs`

all: keypress database clean

keypress: keypress.c
	$(CC) $(CFLAGS) $(EXECS1) keypress.c $(DLFLAGS)

database: Database_Integration.c
	$(CC) $(CFLAGS) $(EXECS2) Database_INtegration.c $(DLFLAGS2)

clean:
	-rm -rf *.dSYM

spotless:
	-rm $(EXECS1)
	-rm $(EXECS2)
