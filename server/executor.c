/* Execute xml parsing/updating here*/

#include "executor.h"

void executor_prepare() {
    init_command_queue();
    init_response_queue();
}
void executor_init() {
    pthread_t executor_threads[2];
    /* Launch the executor threads*/
    pthread_create(&executor_threads[0], NULL, &parse_and_execute, NULL);
    pthread_create(&executor_threads[1], NULL, &respond, NULL);
}
static void parse_and_execute(void *arg) {
    while(1) {
        cmd executable_cmd;
        resp command_response;
        dequeue_command(&executable_cmd);
        execute_command(executable_cmd.cmd_no, &command_response.msg);
        
    }
}
static void respond(void *arg);
void execute_command(int cmd_no, char **cmd_response);
void update_train_delay(void *train_table);
void get_next_departures(void *train_table);
void get_next_arrivals(void *train_table);