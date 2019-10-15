/* echo_client.c: Message Queue Echo Client test */

#include "mq/client.h"

#include <assert.h>
#include <time.h>
#include <unistd.h>

/* Constants */

char TOPIC[BUFSIZ];

/* Threads */

void *incoming_thread(void *arg) {
    MessageQueue *mq = (MessageQueue *)arg;

    while (!mq_shutdown(mq)) {
    	char *message = mq_retrieve(mq);
        char* user = getenv("USER");
	if (message) {
            if(!strstr(message, user)) {
                printf("%s\n", message);
            }
	    free(message);
	}
    }

    return NULL;
}

void *outgoing_thread(void *arg) {
    MessageQueue *mq = (MessageQueue *)arg;
    char body[BUFSIZ];
    char* user = getenv("USER");

    printf("\nTo exit, enter \"exit\" or \"quit\"\n\n");

    while(1) {

        /* Get user's message to post */

        printf("%s/ ", user);
        fgets(body, BUFSIZ, stdin);
        if(strstr(body, "exit") || strstr(body, "quit") ) {
            break;
        }

        /* Post message */

        char message[BUFSIZ];
        sprintf(message, "%s/%s", user, message);

        mq_publish(mq, TOPIC, message);
    }
    
    mq_stop(mq);
    return NULL;
}

/* Main execution */

int main(int argc, char *argv[]) {
    /* Parse command-line arguments */
    char *name = getenv("USER");
    char *host = "129.74.152.125";
    char *port = "9614";

    if (argc > 1) { host = argv[1]; }
    if (argc > 2) { port = argv[2]; }
    if (!name)    { name = "test_chat";  }

    /* Create and start message queue */
    MessageQueue *mq = mq_create(name, host, port);

    printf("Pick a topic: ");
    scanf("%s", TOPIC);

    mq_subscribe(mq, TOPIC);
    mq_start(mq);

    /* Run and wait for incoming and outgoing threads */
    Thread incoming;
    Thread outgoing;
    thread_create(&incoming, NULL, incoming_thread, mq);
    thread_create(&outgoing, NULL, outgoing_thread, mq);
    thread_join(incoming, NULL);
    thread_join(outgoing, NULL);

    mq_delete(mq);
    return 0;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */ 
