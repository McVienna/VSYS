/* myclient.c, adapted to c++ */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <string>

#include "protocols.h"
#include "socketutility.h"
#include "userHandlerIO.h"

#define BUF 1024
#define PORT 6552

using namespace std;

int main (int argc, char **argv) {
//Create Socket
  int socket_fd;
  int userInput;
  char* buffer = new char[BUF]; 
  struct sockaddr_in address;
  int size;
  unsigned int transmission_length;
  bool shallContinue = true;

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
    delete buffer;



    
//Communication with Server
  do
    {
      Protocol* protocol;

      printMenu();
      userInput = getRequestInput();
      
      /*
      -1: error
      0: Send_prot
      1: List_prot
      2: Read_prot
      3: Delete_prot
      */
      switch(userInput)
      {
        case 0:
          cout << "SEND A NEW MESSAGE" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new Send_prot();
          break;

        case 1:
          cout << "LIST ALL MESSAGES" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new List_prot();
          break;

        case 2:
          cout << "READ A MESSAGE" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new Read_prot();
          break;

        case 3:
          cout << "DELETE A MESSAGE" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new Delete_prot();
          break;

        case -1:
        default:
          shallContinue = false;
          cout << "Terminating program..." << endl;
          continue; //jump to end of loop
          break;
      }

      //Check Buffer size, realloc, if neccassary.
      transmission_length = protocol->get_buffersize();
      char* buffer = new char[transmission_length]();
      
      protocol->serialize(buffer);
      if (sendall(socket_fd, buffer, transmission_length) == -1)
        {
          perror("sendall");
          printf("We only sent %d bytes because of the error!\n", transmission_length);
        }

      delete buffer;
      delete protocol;
      
    } 
  while (shallContinue == true);

  close (socket_fd);
  return EXIT_SUCCESS;
}
