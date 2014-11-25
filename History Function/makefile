
#Thisnext linedefines arguments that are passed to all compilation steps

CCFLAGS = -g -Wall

simulate: list.o history.o main.o 
	gcc $(CCFLAGS) -o simulate list.o history.o main.o

list.o: list.c list.h
	gcc $(CCFLAGS) -c list.c

history.o: history.c history.h
	gcc $(CCFLAGS) -c history.c

main.o: main.c 
	gcc $(CCFLAGS) -c main.c
	
clean:
	-rm list.o history.o main.o simulate

