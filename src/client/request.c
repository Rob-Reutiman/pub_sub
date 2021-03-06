/* request.c: Request structure */

#include "mq/request.h"

#include <stdlib.h>
#include <string.h>

/**
 * Create Request structure.
 * @param   method      Request method string.
 * @param   uri         Request uri string.
 * @param   body        Request body string.
 * @return  Newly allocated Request structure.
 */
Request * request_create(const char *method, const char *uri, const char *body) {

    Request *r = calloc((size_t)1, sizeof(Request));

    r->method = strdup(method);
    r->uri = strdup(uri);
    if(body) {
        r->body = strdup(body);
    }

    return r;
}

/**
 * Delete Request structure.
 * @param   r           Request structure.
 */
void request_delete(Request *r) {

    if(r->method) {
        free(r->method);
    }

    if(r->uri) {
        free(r->uri);
    }

    if(r->body) {
        free(r->body);
    }

    r->next = NULL;
    
    free(r);

}

/**
 * Write HTTP Request to stream:
 *  
 *  $METHOD $URI HTTP/1.0\r\n
 *  Content-Length: Length($BODY)\r\n
 *  \r\n
 *  $BODY
 *      
 * @param   r           Request structure.
 * @param   fs          Socket file stream.
 */
void request_write(Request *r, FILE *fs) {

    size_t len = 0;

    if(r->body) {
        len = strlen(r->body) * sizeof(r->body[0]);
    }
    
    char buffer[BUFSIZ];

    if(len > 0) {
        sprintf(buffer, "%s %s HTTP/1.0\r\nContent-Length: %lu\r\n\r\n%s", r->method, r->uri, len, r->body);
    } else {
        sprintf(buffer, "%s %s HTTP/1.0\r\n\r\n", r->method, r->uri);
    }

    fprintf(fs, "%s", buffer);

}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */ 
