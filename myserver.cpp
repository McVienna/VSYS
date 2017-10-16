/* myserver.c, adapted to c++ */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "protocols.h"
#include "s_filehandler.h"

#define BUF 1024
#define PORT 6543

using namespace std;

int main (void) {
//Create Socket
  int server_socket_fd, client_socket_fd;
  socklen_t addrlen;
  char buffer[BUF];
  int size;
  struct sockaddr_in address, cliaddress;

  server_socket_fd = socket (AF_INET, SOCK_STREAM, 0);

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons (PORT);

  if (bind ( server_socket_fd, (struct sockaddr *) &address, sizeof (address)) != 0)
    {
      perror("bind error");
      return EXIT_FAILURE;
    }
  listen (server_socket_fd, 5);
  
  addrlen = sizeof (struct sockaddr_in);

//Main Program, runs until killed.
  while (1)
    { //Wait for Connection
      printf("Waiting for connections...\n");
      client_socket_fd = accept (server_socket_fd, (struct sockaddr *) &cliaddress, &addrlen) ;
      if (client_socket_fd > 0)
        {
          printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
          strcpy(buffer,"Welcome to TWMailer, Please enter your command:\n");
          send(client_socket_fd, buffer, strlen(buffer),0);
        }
      //Communication with Client
      do
        {
          size = recv (client_socket_fd, buffer, BUF-1, 0);
          if( size > 0)
            {
              buffer[size] = '\0';
              printf ("Message received: %s\n", buffer);
            }
          else if (size == 0)
            {
              printf("Client closed remote socket\n");
              break;
            }
          else
            {
              perror("recv error");
              return EXIT_FAILURE;
            }
        }
      while (strncmp (buffer, "quit", 4)  != 0);
      close (client_socket_fd);
    }
  close (server_socket_fd);
  return EXIT_SUCCESS;
}
