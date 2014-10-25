#ifndef _common_h_
#define _common_h_

#include <stdio.h>
#include <stdlib.h>
#define MAXCRS 7
#define MAXLEN 20
#define MAXQUERY 1024

/* Queue Data Structure */
typedef struct Node_t {
	void *data;
	struct Node_t *prev, *next;
} Node_t;

typedef struct Queue_t {
	Node_t *head;
	Node_t *tail;
	int size;
} Queue_t;


int init(Queue_t *);
void destroy(Queue_t *);
int enqueue(Queue_t *, void *);
int dequeue(Queue_t *, void **);
int next(Queue_t *, void **, void **);
int size(Queue_t *);
int ieEmpty(Queue_t *);
/* End of Queue Data Structure */


/* Database Integration */

/* End of Database Itegration */




#endif
