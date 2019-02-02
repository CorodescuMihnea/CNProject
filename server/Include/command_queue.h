#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <stdlib.h>
#include <string.h>                                            
#include <sys/queue.h> 
#include <pthread.h>
#include <stdio.h>

#define BAD_COMMAND -1
#define DELAY 0
#define ARIVALS 1
#define DEPARTURES 2

typedef struct _command {
   int cli_sd;
   int cmd_no;
   char **args;
   STAILQ_ENTRY(_command) cmd_pointers;
}cmd;

STAILQ_HEAD(command_queue, _command);
struct command_queue g_command_queue;
pthread_mutex_t g_command_queue_lock;


void init_command_queue();
void enqueue_command(void *client_command);
int dequeue_command(cmd *output_command);
#endif