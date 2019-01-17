#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <pthread.h>
#include <libxml/parser.h>
#include <unistd.h>
#include <stdio.h>

#include "command_queue.h"
#include "response_queue.h"
#include "utils.h"

void executor_init();

void parse_and_execute();
void respond();
void execute_command(int cmd_no, char **cmd_response);

void update_train_delay(void *train_table);
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);
#endif