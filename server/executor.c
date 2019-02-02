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
void parse_and_execute() {
    while(1) {
        cmd *executable_cmd;
        executable_cmd = (cmd *)malloc(sizeof(cmd));
        resp *command_response;
        command_response = (resp *)malloc(sizeof(resp));

        if (dequeue_command(executable_cmd) == 0) {
            free(command_response);
            free(executable_cmd);
            continue;
        }

        command_response->cli_sd = executable_cmd->cli_sd;
        command_response->msg = (char *) malloc(512);

        execute_command(executable_cmd->cmd_no, executable_cmd->args, command_response->msg);
        free(executable_cmd);
        enqueue_response(command_response);
    }
}
void respond() {
    while(1) {
        resp *server_response;
        server_response = (resp *)malloc(sizeof(resp));
        server_response->msg = (char *)malloc(512);

        if (dequeue_response(server_response) == 0) {
            free(server_response->msg);
            free(server_response);
            continue;
        }
        printf("[Executor.response_thread - Writing '%s' to client\n", server_response->msg);
        write(server_response->cli_sd, server_response->msg, sizeof(server_response->msg));
        close(server_response->cli_sd);
        free(server_response);
    }
}
void execute_command(int cmd_no, char **argv, char *cmd_response) {
    if(cmd_no == DELAY) { 
        char resp[] = "ok";
        strcpy(cmd_response, resp);
    }
    else if(cmd_no == ARIVALS) {
        char resp[] = "<train>\n</train>\n<train>\n</train>\n<train>\n</train>\n";
        strcpy(cmd_response, resp);
    }
    else if(cmd_no == DEPARTURES) {
        char resp[] = "<train>\n</train>\n<train>\n</train>\n<train>\n</train>\n";
        strcpy(cmd_response, resp);
    }
    else if(cmd_no == BAD_COMMAND) {
        char resp[] = "Bad command: available options are [delay 'args']|[arivals]|[departures]";
        strcpy(cmd_response, resp);
    }
}

void update_train_delay(void *train_table);
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);
