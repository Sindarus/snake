/**
* \file queue.c
* \brief Queue struct definition and functions related.
*/

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "queue.h"

/**
* \struct queue
* \brief FILO queue with data of type 'direction'.
*/
struct queue{
    direction* data;
    int size;
    int start;
    int end;
    bool is_full;
};

/**
* \fn queue new_queue(int size);
* \brief Used to create a variable of type 'queue'
* \details This is a FILO queue that can store 'direction' variables
* \param size maximum size of the queue
* \returns The newly created queue.
*/
queue new_queue(int size){
    queue q;
    q.data = malloc(size * sizeof(direction));
    q.size = size;
    q.start = 1;
    q.end = 1;
    q.is_full = false;
    return q;
}

/**
* \fn bool queue_empty(queue* q);
* \brief Used to know if a queue is empty or not.
* \returns True if the passed queue is empty, false otherwise.
*/
bool queue_empty(queue* q){
    if(q->is_full) return 0;
    return (q->start == q->end);
}

/**
* \fn bool queue_full(queue* q);
* \brief Used to know if a queue is full or not.
* \returns True if the passed queue is full, false otherwise.
*/
bool queue_full(queue* q){
    return q->is_full;
}

/**
* \fn void enqueue(queue* q, direction elt);
* \brief Used to add an element at the end of the queue
* \param q Pointer to the queue to which to add the element
* \param elt Element to add to the queue
*/
void enqueue(queue* q, direction elt){
    if(q->is_full){
        printf("Queue is full, can't enqueue.\n");
        exit(1);
    }

    q->data[q->end] = elt;

    q->end++;
    if(q->end >= q->size){
        q->end = 0;
    }

    if(q->start == q->end) q->is_full = true;
}

/**
* \fn direction dequeue(queue* q);
* \brief Used to retrieve the first element of a queue.
* \param q A pointer to the queue from which to get the first element.
* \returns The first element of the queue passed
*/
direction dequeue(queue* q){
    if(queue_empty(q)){
        printf("Queue is empty, can't dequeue.\n");
        exit(1);
    }

    direction res = q->data[q->start];
    q->start++;
    if(q->start >= q->size){
        q->start = 0;
    }

    if(q->is_full) q->is_full = false;

    return res;
}

/**
* \fn void display_queue_int(queue q);
* \brief Used to display the content of a queue as if they were integers.
* \details This function is non-destructive.
* \param q The queue to display.
*/
void display_queue_int(queue q){
    direction cur_elt;

    printf("< ");
    while(!queue_empty(&q)){
        cur_elt = dequeue(&q);
        printf("%i ", cur_elt);
    }
    printf("<\n");
}