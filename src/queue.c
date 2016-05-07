#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "queue.h"

struct queue{
    direction* data;
    int size;
    int start;
    int end;
    bool is_full;
};

queue new_queue(int size){
    queue q;
    q.data = malloc(size * sizeof(direction));
    q.size = size;
    q.start = 1;
    q.end = 1;
    q.is_full = false;
    return q;
}

bool queue_empty(queue* q){
    if(q->is_full) return 0;
    return (q->start == q->end);
}

bool queue_full(queue* q){
    return q->is_full;
}

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

void display_queue_int(queue q){
    direction cur_elt;

    printf("< ");
    while(!queue_empty(&q)){
        cur_elt = dequeue(&q);
        printf("%i ", cur_elt);
    }
    printf("<\n");
}