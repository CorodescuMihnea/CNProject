#ifndef RESPONSE_QUEUE_H
#define RESPONSE_QUEUE_H
                                           
#include <stdlib.h>
#include <string.h>                                            
#include <pthread.h>
#include <sys/queue.h>   

typedef struct _response {
   int cli_sd;
   char *msg;
   STAILQ_ENTRY(_response) resp_pointers;
}resp;

STAILQ_HEAD(response_queue, _response);
struct response_queue g_response_queue;
pthread_mutex_t g_response_queue_lock;


void init_response_que();
void enqueue_response(void *server_response);
void dequeue_response(resp *output_response);
#endif