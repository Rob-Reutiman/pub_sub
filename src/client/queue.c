/* queue.c: Concurrent Queue of Requests */

#include "mq/queue.h"

/**
 * Create queue structure.
 * @return  Newly allocated queue structure.
 */
Queue * queue_create() {

    Queue* q = calloc((size_t)1, sizeof(Queue));

    return q;
}

/**
 * Delete queue structure.
 * @param   q       Queue structure.
 */
void queue_delete(Queue *q) {

    while(q->size) {

        Request *r = q->head;
        q->head = r->next;
        /*
        if(q->size = 1) {
            q->tail = NULL;
        }
          do i even need this
        */

        request_delete(r);
        q->size--;
    }

    free(q);

}

/**
 * Push request to the back of queue.
 * @param   q       Queue structure.
 * @param   r       Request structure.
 */
void queue_push(Queue *q, Request *r) {

    // while(q ! full)
    // need to spin, lock, test and set, and yield
    // plus cond_wait so others can go
    // plus cond_signal so know when ready

    if(!q->head) {
        q->head = r;
    }

    q->tail->next = r;
    q->tail = r;
    q->size++;

}

/**
 * Pop request to the front of queue (block until there is something to return).
 * @param   q       Queue structure.
 * @return  Request structure.
 */
Request * queue_pop(Queue *q) {

    //while(q->size == 0); 
    // need to spin, lock, test and set, and yield
    // plus cond_wait so others can go
    // plus cond_signal so know when ready

    Request *r = q->head;
    q->head = r->next;
    q->size--;

    return r;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
