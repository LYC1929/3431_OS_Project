
#Thisnext linedefines arguments that are passed to all compilation steps

CCFLAGS = -g -Wall

simulate: list.o history.o
	gcc $(CCFLAGS) -o simulate list.o history.o

list.o: list.c list.h
	gcc $(CCFLAGS) -c list.c

history.o: history.c 
	gcc $(CCFLAGS) -c history.c

clean:
	-rm list.o history.o simulate

