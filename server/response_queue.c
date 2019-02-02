#include "response_queue.h"

void init_response_queue() {
    STAILQ_INIT(&g_response_queue);
    pthread_mutex_init(&g_response_queue_lock, NULL);
}

void enqueue_response(void *server_response) {
    resp *serv_resp;
    serv_resp = (resp *)server_response;
    pthread_mutex_lock(&g_response_queue_lock);
    printf("[Executor.response_thread] Enqueueing response %s intended for client with sd %d\n", serv_resp->msg, serv_resp->cli_sd);
    STAILQ_INSERT_TAIL(&g_response_queue, serv_resp, resp_pointers); 
    pthread_mutex_unlock(&g_response_queue_lock); 
    return;
}

int dequeue_response(resp *output_response) {
    pthread_mutex_lock(&g_response_queue_lock); 
    if(!STAILQ_EMPTY(&g_response_queue)) {
        resp *out_resp;
        out_resp = STAILQ_FIRST(&g_response_queue);
        output_response->cli_sd = out_resp->cli_sd;
        strcpy(output_response->msg, out_resp->msg);
        printf("[Executor.response_thread] Dequeued response %s intended for client with sd %d\n", output_response->msg, output_response->cli_sd);
        STAILQ_REMOVE_HEAD(&g_response_queue, resp_pointers);
    }
    else {
        pthread_mutex_unlock(&g_response_queue_lock);
        return 0;
    }
    pthread_mutex_unlock(&g_response_queue_lock);
    return 1;
}
