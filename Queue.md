Accessing the inside of the queue
--------------------------------

- `next` -- for iterating over the queue
  where the position was found using `next`

The function use a "context" parameter to let the queue.c
functions remember where they are within the queue. `next`
changes the context variable.

To iterate through the linked queue, begin with the context as a
NULL value.  Sample iteration code is:

    Queue_t the_queue;
    void *context = NULL;
    void *mydata;

    /* Assume that the queue has been inialized and data has been added to it. */

    while ( next( &the_queue, &context, &mydata ) && (mydata != NULL)) {
      /* Do something with the data pointed to by mydata. */
    }

