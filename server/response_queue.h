#ifndef RESPONSE_QUEUE_H
#define RESPONSE_QUEUE_H
                                           
#include <stdlib.h>
#include <string.h>                                            
#include <sys/queue.h>   

typedef struct response_queue {
    char response[256];
    STAILQ_ENTRY(rqueue) pointers;
}rqueue;
void enqueue_response(void *eq);
void dequeue_response(void *eq, char* response);
#endif