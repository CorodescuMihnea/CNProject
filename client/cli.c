#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

extern int errno;

int port;

int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	

  if (argc != 3)
    {
      printf ("Syntax: %s <server_addr> <port>\n", argv[0]);
      return -1;
    }


  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Error at socket().\n");
      return errno;
    }


  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Error at connect().\n");
      return errno;
    }

  printf ("[client]Type a command: ");
  fflush (stdout);

  char cmd[128];
  int ret;
  memset(cmd, 0, sizeof(cmd));
  ret = read (0, cmd, sizeof(cmd) - 1);
  cmd[ret - 1] = '\0';
  
  printf("[client]Writing to server %s\n", cmd);
  fflush(stdout);

  if (write (sd, cmd, sizeof(cmd)) <= 0)
    {
      perror ("[client]Error at write() to server.\n");
      return errno;
    }

  char resp[512];
  if ((ret = read (sd, resp, sizeof(resp) - 1)) <= 0)
    {
      perror ("[client]Error at read() from server.\n");
      return errno;
    }
  resp[ret] = '\0';

  printf ("[client]Read %d from server .The response is: %s\n",ret, resp);

  close (sd);
}