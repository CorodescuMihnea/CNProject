#include "response_queue.h"

void init_response_queue() {
    STAILQ_INIT(&g_response_queue);
    pthread_mutex_init(&g_response_queue_lock, NULL);
}

void enqueue_response(void *server_response) {
    resp *serv_resp;
    serv_resp = (resp *)server_response;
    pthread_mutex_lock(&g_response_queue_lock); 
    STAILQ_INSERT_TAIL(&g_response_queue, serv_resp, resp_pointers); 
    pthread_mutex_unlock(&g_response_queue_lock); 
}

void dequeue_response(resp *output_response) {
    pthread_mutex_lock(&g_response_queue_lock); 
    if(!STAILQ_EMPTY(&g_response_queue)) {
        output_response = STAILQ_FIRST(&g_response_queue);
        STAILQ_REMOVE_HEAD(&g_response_queue, resp_pointers);
    }
    pthread_mutex_unlock(&g_response_queue_lock); 
}
