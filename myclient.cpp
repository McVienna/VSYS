/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "protocols.h"

#define BUF 1024
#define PORT 6543

using namespace std;

int main (int argc, char **argv) {
//Create Socket
  int socket_fd;
  char buffer[BUF];
  struct sockaddr_in address;
  int size;

  //Check Arguments
  if( argc < 2 )
    {
      printf("Usage: %s ServerAdresse\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  if ((socket_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("Socket error");
      return EXIT_FAILURE;
    }
  
  memset(&address,0,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons (PORT);
  inet_aton (argv[1], &address.sin_addr);

  if (connect (socket_fd, (struct sockaddr *) &address, sizeof (address)) == 0)
    {
      printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
      size=recv(socket_fd,buffer,BUF-1, 0);
      if (size>0)
        {
          buffer[size]= '\0';
          printf("%s",buffer);
        }
    }
  else
    {
      perror("Connect error - no server available");
      return EXIT_FAILURE;
    }
//Communication with Server
  do
    {
      printf ("Send Request: ");
      fgets (buffer, BUF, stdin);
      //evaluate User Protocoll and put data onto buffer.
      //buffer = 
      send(socket_fd, buffer, strlen (buffer), 0);
    } 
  while (strcmp (buffer, "quit\n") != 0);

  close (socket_fd);
  return EXIT_SUCCESS;
}
