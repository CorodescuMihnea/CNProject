#include "command_queue.h"

void init_command_queue() {
    STAILQ_INIT(&g_command_queue);
    pthread_mutex_init(&g_command_queue_lock, NULL);
}

void enqueue_command(void *client_command) {
    cmd *cli_cmd;
    cli_cmd = (cmd *)client_command;
    pthread_mutex_lock(&g_command_queue_lock);
    printf("[Executor.executing_thread] Enqueued command %d from client with sd %d\n", cli_cmd->cmd_no, cli_cmd->cli_sd);
    STAILQ_INSERT_TAIL(&g_command_queue, cli_cmd, cmd_pointers); 
    pthread_mutex_unlock(&g_command_queue_lock);
    return;
}

int dequeue_command(cmd *output_command) {
    pthread_mutex_lock(&g_command_queue_lock); 
    if(!STAILQ_EMPTY(&g_command_queue)) {
        memcpy(output_command ,STAILQ_FIRST(&g_command_queue), sizeof(cmd));
        printf("[Executor.executing_thread] Dequeued command %d from client with sd %d\n", output_command->cmd_no, output_command->cli_sd);
        STAILQ_REMOVE_HEAD(&g_command_queue, cmd_pointers);
    }
    else {
        pthread_mutex_unlock(&g_command_queue_lock); 
        return 0;
    }
    pthread_mutex_unlock(&g_command_queue_lock); 
    return 1;
}
