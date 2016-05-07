/**
* \file queue.h
*/

#ifndef H_QUEUE
#define H_QUEUE

#include <stdbool.h>
#include "types.h"

typedef   struct queue   queue;

queue new_queue(int size);
bool queue_empty(queue* q);
void enqueue(queue* q, direction elt);
direction dequeue(queue* q);
void display_queue_int(queue q);

#endif

