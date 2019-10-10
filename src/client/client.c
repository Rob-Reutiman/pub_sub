/* client.c: Message Queue Client */

#include "mq/client.h"
#include "mq/logging.h"
#include "mq/socket.h"
#include "mq/string.h"

/* External Functions */

char* host = "localhost";
char* port = "9620";

/**
 * Create Message Queue withs specified name, host, and port.
 * @param   name        Name of client's queue.
 * @param   host        Address of server.
 * @param   port        Port of server.
 * @return  Newly allocated Message Queue structure.
 */
MessageQueue * mq_create(const char *name, const char *host, const char *port) {

    MessageQueue *mq = calloc((size_t)1, sizeof(MessageQueue));

    strcpy(mq->name, name);
    strcpy(mq->host, host);
    strcpy(mq->port, port);

    mq->incoming = queue_create();
    mq->outgoing = queue_create();

    mutex_init(&(mq->lock), NULL);

    return mq;
}

/**
 * Delete Message Queue structure (and internal resources).
 * @param   mq      Message Queue structure.
 */
void mq_delete(MessageQueue *mq) {

    queue_delete(mq->incoming);
    queue_delete(mq->outgoing);
    
    free(mq);

}

/**
 * Publish one message to topic (by placing new Request in outgoing queue).
 * @param   mq      Message Queue structure.
 * @param   topic   Topic to publish to.
 * @param   body    Message body to publish.
 */
void mq_publish(MessageQueue *mq, const char *topic, const char *body) {

    char uri[BUFSIZ];
    char* method = "PUT";
    sprintf(uri, "/topic/%s", topic);

    Request *r = request_create(method, uri, body);

    queue_push(mq->outgoing, r);

}

/**
 * Retrieve one message (by taking Request from incoming queue).
 * @param   mq      Message Queue structure.
 * @return  Newly allocated message body (must be freed).
 */
char * mq_retrieve(MessageQueue *mq) {

    Request *r = queue_pop(mq->incoming);

    char* new_body = strdup(r->body);
    request_delete(r);

    return new_body;
}

/**
 * Subscribe to specified topic.
 * @param   mq      Message Queue structure.
 * @param   topic   Topic string to subscribe to.
 **/
void mq_subscribe(MessageQueue *mq, const char *topic) {

    char uri[BUFSIZ];
    sprintf(uri, "/subscription/%s/%s", mq->name, topic);
    char* method = "PUT";

    Request *r = request_create(method, uri, NULL);

    queue_push(mq->outgoing, r);

}

/**
 * Unubscribe to specified topic.
 * @param   mq      Message Queue structure.
 * @param   topic   Topic string to unsubscribe from.
 **/
void mq_unsubscribe(MessageQueue *mq, const char *topic) {

    char uri[BUFSIZ];
    sprintf(uri, "/subscription/%s/%s", mq->name, topic);
    char* method = "DELETE";

    Request *r = request_create(method, uri, NULL);

    queue_push(mq->outgoing, r);

}

/**
 * Start running the background threads:
 *  1. First thread should continuously send requests from outgoing queue.
 *  2. Second thread should continuously receive reqeusts to incoming queue.
 * @param   mq      Message Queue structure.
 */
void mq_start(MessageQueue *mq) {

    thread_create(&(mq->pusher), NULL, &push_func, (void*)mq);  
    thread_create(&(mq->puller), NULL, &pull_func, (void*)mq);

}

/**
 * Stop the message queue client by setting shutdown attribute and sending
 * sentinel messages
 * @param   mq      Message Queue structure.
 */
void mq_stop(MessageQueue *mq) {

    mq->shutdown = true;
    
    int push_r = 0; // r for result
    int pull_r = 0;

    thread_join(mq->pusher, (void **)&push_r);
    thread_join(mq->puller, (void **)&pull_r);

    //send sentinel messages
  //  char uri[BUFSIZ];
  //  sprintf(uri, "/subscription/%s", mq->name);

 //   Request *r = request_create("DELETE", uri, body);

 //   queue_push(mq->outgoing, r);

}

/**
 * Returns whether or not the message queue should be shutdown.
 * @param   mq      Message Queue structure.
 */
bool mq_shutdown(MessageQueue *mq) {

    // lock here and check shutdown
    
    bool is_down = false;

    mutex_lock(&(mq->lock)); 
    if(mq->shutdown) {
        is_down = true;
    }
    mutex_unlock(&(mq->lock));

    return is_down; 
}

/**
 * Continuously sends requests from outgoing queue
 * @param   mq      Message Queue structure.
 */
void* push_func(void *m) {

    MessageQueue *mq =(MessageQueue *)m;

    FILE* fs;

    while(!mq_shutdown(mq)) {

        Request *r = queue_pop(mq->outgoing);

        if( (fs = socket_connect(mq->host, mq->port)) ) {
            request_write(r, fs);
            printf("push request written\n\n");

// do i need this

            char response[BUFSIZ];
            if( !fgets(response, BUFSIZ, fs) ) {
                // free everything and exit failure..?
            } else {
                char* status = strtok(response, "\n");
                if(!strcmp(status, "HTTP/1.0 404 Not Found")) {                   
                    // Content message
                    // what do i do here
                    queue_push(mq->outgoing, r); 
                }
            } 
// to here
        } else {
            queue_push(mq->outgoing, r); // add to outgoing if fail to write to socket
        }
    }

    return (void *) 0;
}

/**
 * Continuously recieves requests from incoming queue
 * @param   mq      Message Queue structure.
 */
void* pull_func(void *m) {

    MessageQueue *mq = (MessageQueue *)m;

    FILE* fs;

    char uri[BUFSIZ];
    sprintf(uri, "/queue/%s", mq->name);
    char* method= "GET";

    while(!mq_shutdown(mq)) {

        Request *r = request_create(method, uri, NULL);

        if( (fs = socket_connect(mq->host, mq->port)) ) {
            request_write(r, fs);
            char response[BUFSIZ];
            if( !fgets(response, BUFSIZ, fs) ) {
                // goto fail
            } else {
                printf("%s", response);
                if(!strstr(response, "200")) { 
                    // i think nothing goes here

                } else {
                    printf("this is chill\n");
                 /*   char* body;
                    while (fgets(body, BUFSIZ, fs)) { // we wanna run her til bod

                    }
                    Request* to_add = request_create(method, uri, body);
                    queue_push(mq->incoming, to_add); */
                }
            } 
        } 
            
        request_delete(r);
    
    }

    return (void *)0;

}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
