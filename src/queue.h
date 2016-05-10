/**
* \file queue.h
*/

#ifndef H_QUEUE
#define H_QUEUE

#include <stdbool.h>
#include "types.h"

/**
* \typedef queue
* \brief FILO queue with data of type 'direction'.
*/
typedef struct queue{
    direction* data;
    int size;
    int start;
    int end;
    bool is_full;
} queue;

queue new_queue(int size);
bool queue_empty(queue* q);
bool queue_full(queue* q);
void enqueue(queue* q, direction elt);
direction dequeue(queue* q);
void display_queue_int(queue q);
void free_queue(queue* q);

#endif

