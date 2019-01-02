#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <pthread.h>
#include <libxml/parser.h>

#include "command.h"

typedef struct command {
    int cl_id;
    char *commmand;
}cmd;

typedef struct executor_queue {
    cmd cmd_queue;
    pthread_mutex_t lock;
}equeue;

void update_train_delay(void *train_table );
void * get_next_departures(void *train_table);
void * get_next_arrivals(void *train_table);
#endif