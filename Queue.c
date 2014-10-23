#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"


/* initialize a Queue data structure */
int init(Queue_t *queue)
{
	int all_ok = 0;

	if (queue != NULL)
	{
		queue->head = NULL;
		queue->tail = NULL;
		queue->size = 0;
		all_ok = 1;
	}
	return all_ok;
}

/* delete all elements in a queue */

void destroy(Queue_t *queue)
{
	Node_t *tmp = NULL;

	if (queue->head != NULL)
	{
		while (queue->head != NULL)
		{
			tmp = queue->head;
			queue->head = queue->head->next;
			free(tmp);
		}
		queue->head = NULL;
		queue->tail = NULL;
	}
}

/* add an element to the end of the queue */
int enqueue(Queue_t *queue, void *data)
{
	int all_ok = 0;
	if (queue != NULL)
	{
		Node_t *n= (Node_t *) malloc(sizeof(Node_t));
		n->next = NULL;
		n->prev = NULL;
		n->data = data;
		if (queue != NULL && n != NULL)
		{
			n->next = NULL;
			n->prev = queue->tail;

			if (queue->tail == NULL)
				queue->head = n;
			else
				queue->tail->next = n;
			queue->size += 1;
			all_ok = 1;
		}
	}
	return all_ok;
}

/* remove the first element in a queue */
int dequeue(Queue_t *queue, void **data)
{
	int all_ok = 0;

	Node_t *tmp = NULL;

	if (queue != NULL && data != NULL)
	{
		if (queue->head != NULL)
		{
			/* get the data */
			*data = queue->head->data;

			/* remove the node */
			tmp = queue->head;
			queue->head = queue->head->next;
			if (queue->head == NULL)
				queue->tail = NULL;
			else
				queue->head->prev = NULL;

			free(tmp);
			all_ok = 1;
		}
		else
			*data = NULL;
		queue->size -= 1;
	}

	return all_ok;
}

/* Allow code to traverse through a queue linearly.  The context
   parameter is state that the calling program stores for the
   traversal.  It indicates the last node in the queue that was
   reported by the traversal.  To begin a traversal, pass a NULL
   value as the context content (but not the context variable itself). */

int next(Queue_t *queue, void **context, void **data)
{
	int all_ok = 1;

	Node_t **curr = (Node_t **) context;

	/* determine the next node to report in the queue */
	if (*curr == NULL)
		*curr = queue->head;
	else
		*curr = (*curr)->next;

	/* if we haven't hit the end of the queue, report the data */

	*data = NULL;
	if (*curr != NULL)
		*data = (*curr)->data;

	return all_ok;
}

/* return the size of the queue */
int size(Queue_t *queue) { return queue->size; }

/* determine if the queue is empty */
int isEmpty(Queue_t *queue) { return (queue->size == 0); }




