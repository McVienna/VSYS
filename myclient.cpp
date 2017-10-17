/* myclient.c, adapted to c++ */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "protocols.h"
#include "socketutility.h"

#define BUF 1024
#define PORT 6543

using namespace std;

int main (int argc, char **argv) {
//Create Socket
  int socket_fd;
  std::string userInput = "";
  char* buffer = ((char*) malloc(BUF*sizeof(char)));
  struct sockaddr_in address;
  int size;
  unsigned int transmission_length;

  //check buffer-allocation
  if (buffer == NULL)
  {
    // error handling 
    printf("Allocation of memory has failed. Probably not your fault :(");
    exit(-1);
  }


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
  
  memset(&address, 0, sizeof(address));
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
      cout << ("Send Request: ");
      cin  >> userInput;

      if(userInput == "SEND")
      {
        //create Protocol Object
        Send_prot protocol;

        //Check Buffer size, realloc, if neccassary.
        if (strlen(buffer) < (unsigned int) protocol.get_buffersize())
          {
            char* tmp_a = (char*) realloc(buffer, protocol.get_buffersize()*sizeof(char));
            if ( tmp_a == NULL ) // realloc has failed
              {
                // error handling
                printf("The re-allocation of array serialized_data has failed. Probably not your fault :(");
                free(buffer);
                exit(-2);
              }
            else
              {
                buffer = tmp_a;
              }
          }
        
        buffer = protocol.serialize();
        transmission_length = (unsigned int) strlen (buffer);
        if(sendall(socket_fd, buffer, transmission_length) == -1)
          {
            perror("sendall");
            printf("We only sent %d bytes because of the error!\n", transmission_length);
          }
      }



      /*
      fgets (buffer, BUF, stdin);
      //evaluate User Protocoll and put data onto buffer.
      transmission_length = (unsigned int) strlen (buffer);
      if(sendall(socket_fd, buffer, transmission_length) == -1)
        {
          perror("sendall");
          printf("We only sent %d bytes because of the error!\n", transmission_length);
        }
        */
    } 
  while (strcmp (buffer, "quit\n") != 0);

  close (socket_fd);
  free(buffer);
  return EXIT_SUCCESS;
}
