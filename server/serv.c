#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
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
    int pid;
    pthread_t client_thread[NOF_ACCEPTABLE_CLIENT_REQUESTS]; //Client thread identifiers
    pthread_t executor_thread; // Thread that listens and executes commands from the queue
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
    ClientThrdData client_thrd;
    client_thrd = *((struct ClientThrdData *)arg);

    printf("[thread]- %d - Waiting input from thread...\n", client_thrd.thread_id);
    fflush(stdout);

    pthread_detach(pthread_self());

    answear((ClientThrdData *)arg);
    /* Done with this client, close the connection */
    close_client_desc(arg);
    return (NULL);
}

int close_client_descriptor(void * cl) {
    ClientThrdData cl_thrd_data;
    cl_thrd_data = *((ClientThrdData *) cl);
    return close(cl_thrd_data.cl);
}