#include "command_queue.h"

void init_command_queue() {
    STAILQ_INIT(&g_command_queue);
    pthread_mutex_init(&g_command_queue_lock, NULL);
}

void enqueue_command(void *client_command) {
    cmd *cli_cmd;
    cli_cmd = (cmd *)client_command;
    pthread_mutex_lock(&g_command_queue_lock); 
    STAILQ_INSERT_TAIL(&g_command_queue, cli_cmd, cmd_pointers); 
    pthread_mutex_unlock(&g_command_queue_lock); 
}

int dequeue_command(cmd *output_command) {
    pthread_mutex_lock(&g_command_queue_lock); 
    if(!STAILQ_EMPTY(&g_command_queue)) {
        output_command = STAILQ_FIRST(&g_command_queue);
        STAILQ_REMOVE_HEAD(&g_command_queue, cmd_pointers);
    }
    else {
        return 0;
    }
    pthread_mutex_unlock(&g_command_queue_lock); 
    return 1;
}
