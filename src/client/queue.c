/* queue.c: Concurrent Queue of Requests */

#include "mq/queue.h"

/**
 * Create queue structure.
 * @return  Newly allocated queue structure.
 */
Queue * queue_create() {

    Queue* q = calloc((size_t)1, sizeof(Queue));

    mutex_init(&(q->lock), NULL);
    cond_init(&(q->cond), NULL);

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
        
        if(q->size == 1) {
            q->tail = NULL;
        } 

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

    mutex_lock(&(q->lock));

    if(!q->head) {
        q->head = r;
        q->tail = r;
    } else {
        q->tail->next = r;
        q->tail = r;
    }
 
    q->size++;

    cond_signal(&(q->cond));
    mutex_unlock(&(q->lock));

}

/**
 * Pop request to the front of queue (block until there is something to return).
 * @param   q       Queue structure.
 * @return  Request structure.
 */
Request * queue_pop(Queue *q) {

    mutex_lock(&(q->lock));

    while(q->size == 0) {
       cond_wait(&(q->cond), &(q->lock));
    } 

    Request *r = q->head;
    q->head = r->next;
    q->size--;

    mutex_unlock(&(q->lock));

    return r;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
