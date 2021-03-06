#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
// #include <unistd.h>
// #include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>

#include "executor.h"
#include "utils.h"

#define PORT 2908
#define NOF_ACCEPTABLE_CLIENT_REQUESTS 50

extern int errno;

typedef struct ClientThrdData {
    int thread_id;
    int cl;       //client descriptor returned by accept
} ClientThrdData;

static void *treat_client(void *);
void answear(void *);

int main() {
    struct sockaddr_in server; //structure used by server
    struct sockaddr_in from;
    int sd; //socket descriptor
    pthread_t client_thread[NOF_ACCEPTABLE_CLIENT_REQUESTS]; //Client thread identifiers
    int thread_cnt = 0; //Thread counter

    /* Create the server socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server]Error creating server socket().\n");
        return errno;
    }
    /*Ussing SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* Prepare data structures */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* Fill the server data structure */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    /* Bind the socket */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        perror("[server]Error at bind().\n");
        return errno;
    }

    /* Listen for clients */
    if (listen(sd, 2) == -1) {
        perror("[server]Error la listen().\n");
        return errno;
    }
    /* Start the executor */
    executor_init();

    /* Concurrently serve clients using threads */
    /* Client connections are not kept alive*/  

    while (1) {
        int client;
        ClientThrdData *td;
        int length = sizeof(from);

        printf("[server]Waiting at port %d...\n", PORT);
        fflush(stdout);

        /* Accepting a client  */
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0) {
            perror("[server]Error at accept().\n");
            continue;
        }

        /* Assigning a thread for each client */
        td = (struct ClientThrdData *)malloc(sizeof(struct ClientThrdData));
        td->thread_id = thread_cnt++;
        td->cl = client;

        pthread_create(&client_thread[thread_cnt], NULL, &treat_client, td);
    }
}

static void *treat_client(void *arg) {
    ClientThrdData *client_thrd;
    client_thrd = (struct ClientThrdData *) arg;

    char client_msg_buff[256];
    memset(client_msg_buff, 0, 256);

    printf("[thread]- %d - Reading command from client\n", client_thrd->thread_id);
    fflush(stdout);
    int ret;
    ret = read(client_thrd->cl, client_msg_buff, sizeof(client_msg_buff));
    printf("[thread]- %d - Read from client %d bytes\n", client_thrd->thread_id, ret);
    printf("[thread]- %d - Read command %s from client\n", client_thrd->thread_id, client_msg_buff);

    cmd *client_cmd;
    client_cmd = (cmd *)malloc(sizeof(cmd));
    memset(client_cmd, 0, sizeof(cmd));
    client_cmd->cli_sd = client_thrd->cl;

    parse_command(client_msg_buff, client_cmd);
    enqueue_command(client_cmd);
    
    pthread_detach(pthread_self());
    return (NULL);
}