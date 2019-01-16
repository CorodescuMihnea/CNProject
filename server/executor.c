/* Execute xml parsing/updating here*/

#include "executor.h"

void executor_init() {
    init_command_queue();
    init_response_queue();
    pthread_t executor_threads[2];
    /* Launch the executor threads*/
    pthread_create(&executor_threads[0], NULL, &parse_and_execute, NULL);
    pthread_create(&executor_threads[1], NULL, &respond, NULL);
}
static void parse_and_execute() {
    while(1) {
        cmd executable_cmd;
        resp *command_response;
        command_response = malloc(sizeof(resp));

        dequeue_command(&executable_cmd);
        command_response->cli_sd = executable_cmd.cli_sd;
        execute_command(executable_cmd.cmd_no, command_response->msg);
        enqueue_respose(&command_response);
    }
}

static void respond() {
    while(1) {
        resp server_response;
        dequeue_response(&server_response);

        close(server_response.cli_sd);
    }
}

void execute_command(int cmd_no, char **cmd_response);
void update_train_delay(void *train_table);
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);
