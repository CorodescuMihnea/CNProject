#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <pthread.h>
#include <libxml/parser.h>

#include "command_queue.h"
#include "response_queue.h"
#include "utils.h"

int executor_init() {
    init_command_queue();
    init_response_queue();
}

int execute_command(int cmd_no);
void update_train_delay(void *train_table );
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);
#endif