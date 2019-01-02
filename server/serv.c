// For thread safety 
// pthread_mutex_lock(&lock); 
// pthread_mutex_unlock(&lock);
// pthread_mutex_t lock; 
// pthread_mutex_init(&lock, NULL)

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

/* portul folosit */
#define PORT 2908
#define NOF_ACCEPTABLE_CLIENT_REQUESTS 50

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct ClientThrdData
{
    int thread_id;
    int cl;       //client descriptor returned by accept
    cqueue *command_q;
    rqueue *response_q;
} ClientThrdData;

static void *treat(void *);
void answear(void *);

int main()
{
    struct sockaddr_in server; //structure used by server
    struct sockaddr_in from;
    int sd; //socket descriptor
    int pid;
    pthread_t client_thrd[NOF_ACCEPTABLE_CLIENT_REQUESTS]; //Client thread identifiers
    pthread_t worker_thrd; // Thread that listens and executes commands from the queueu 
    int thrd_cnt = 0; //Thread counter

    /* Create the server socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
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
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Error at bind().\n");
        return errno;
    }

    /* Listen for clients */
    if (listen(sd, 2) == -1)
    {
        perror("[server]Error la listen().\n");
        return errno;
    }
    /* Start the executor */
    cqueue *cq;
    rqueue *rq;
    if (executor_init(rq, cq) == 0) {
        perror("[server]Failed starting the executor.\n");
        return errno;
    }
    /* Concurrently serve clients using threads */
    /* Client connections are not kept alive*/  

    while (1)
    {
        int client;
        ClientThrdData *td;
        int length = sizeof(from);

        printf("[server]Waiting at port %d...\n", PORT);
        fflush(stdout);

        /* Accepting a client  */
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("[server]Error at accept().\n");
            continue;
        }

        /* Assign a thread for each client */
        td = (struct ClientThrdData *)malloc(sizeof(struct ClientThrdData));
        td->thrd_cnt = i++;
        td->cl = client;
        td->command_q = cq;
        td->response_q = rq;

        pthread_create(&client_thrd[thrd_cnt], NULL, &treat_client, td);
    }
}

static void *treat_client(void *arg)
{
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

void answear(void *arg)
{
    int nr, i = 0;
    char client_msg[CLI_MSG_SIZE];
    char *resp;
    ClientThrdData tdL;
    tdL = *((struct ClientThrdData *)arg);
    /* Read a command from the client */
    if (read(tdL.cl, client_msg, sizeof(client_msg)) <= 0)
    {
        printf("[Thread %d]\n", tdL.thread_id);
        perror("Eroare la read() de la client.\n");
    }
    /* Parse the command and add it to the executor que*/

    /* Prepare the response */
    printf("[Thread %d]Trimitem mesajul inapoi...%s\n", tdL.thread_id, resp);

    /* Send the response to the client */
    if (write(tdL.cl, resp, sizeof(resp)) <= 0)
    {
        printf("[Thread %d] ", tdL.thread_id);
        perror("[Thread]Error at write() to the client\n");
    }
    else
        printf("[Thread %d]Response successfully sent.\n", tdL.thread_id);
}

int close_client_desc(void * cl) {
    ClientThrdData cl_thrd_data;
    cl_thrd_data = *((ClientThrdData *) cl);
    return close(cl_thrd_data.cl);
}