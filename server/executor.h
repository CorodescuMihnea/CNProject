#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <pthread.h>
#include <libxml/parser.h>

#include "command_queue.h"
#include "response_queue.h"
#include "utils"

int executor_init(void *cmd_q, void *resp_q);

int execute_command(int cmd_no);
void update_train_delay(void *train_table );
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);
#endif