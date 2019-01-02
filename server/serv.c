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
#include "command.h"

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData
{
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl;       //descriptorul intors de accept
} thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main()
{
    struct sockaddr_in server; //structure used by server
    struct sockaddr_in from;
    int sd; //socket descriptor
    int pid;
    pthread_t client_th[100]; //Client thread identifiers
    pthread_t worker_th; // Thread that listens and executes commands from the queueu 
    int i = 0;

    /* Create the socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
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
    /* Concurently serve clients using threads */
    while (1)
    {
        int client;
        thData *td; //parametru functia executata de thread
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

        td = (struct thData *)malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&client_th[i], NULL, &treat_client, td);

    }
};
static void *treat_client(void *arg)
{
    struct thData tdL;
    tdL = *((struct thData *)arg);
    printf("[thread]- %d - Waiting input from thread...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData *)arg);
    /* Done with this client, close the connection */
    close(arg);
    return (NULL);
};

void raspunde(void *arg)
{
    int nr, i = 0;
    struct thData tdL;
    tdL = *((struct thData *)arg);
    if (read(tdL.cl, &nr, sizeof(int)) <= 0)
    {
        printf("[Thread %d]\n", tdL.idThread);
        perror("Eroare la read() de la client.\n");
    }

    printf("[Thread %d]Mesajul a fost receptionat...%d\n", tdL.idThread, nr);

    /*pregatim mesajul de raspuns */
    nr++;
    printf("[Thread %d]Trimitem mesajul inapoi...%d\n", tdL.idThread, nr);

    /* returnam mesajul clientului */
    if (write(tdL.cl, &nr, sizeof(int)) <= 0)
    {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread]Eroare la write() catre client.\n");
    }
    else
        printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
}