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
#include <sstream>

#include "protocols.h"
#include "socketutility.h"
#include "userHandlerIO.h"

#define BUF 1024

using namespace std;

int main (int argc, char **argv) {
//Create Socket
  int socket_fd;
  int userInput;
  char* buffer = new char[BUF]; 
  struct sockaddr_in address;
  int size;

  std::vector<char> vec_receive_buffer;

  std::string ldapUsername;

  unsigned short port = 0;

  unsigned int transmission_length;
  bool shallContinue = true;

  //Check Arguments
  if( argc != 3)
  {
    printf("Usage: %s ServerAdresse Port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  istringstream portCheck(argv[2]);
  if (!portCheck >> port)
  {
    printf("Invalid Port!\n");
    exit(EXIT_FAILURE);
  }
  port = atoi(argv[2]);
  if (port < 1024 || port > 65535)
  {
    printf("Invalid Port Range!\n");
    exit(EXIT_FAILURE);
  }

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Socket error");
    return EXIT_FAILURE;
  }
  
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons (port);
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
          protocol = new Send_prot(ldapUsername);
          break;

        case 1:
          cout << "LIST ALL MESSAGES" << endl;
          protocol = new List_prot(ldapUsername);
          break;

        case 2:
          cout << "READ A MESSAGE" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new Read_prot(ldapUsername);
          break;

        case 3:
          cout << "DELETE A MESSAGE" << endl;
          cout << "Please enter required data." << endl << endl;
          protocol = new Delete_prot(ldapUsername);
          break;

        case 4:
          cout << "LOGIN!" << endl;
          cout << "Please enter required data." << endl
               << endl;
          protocol = new Login_prot(ldapUsername);
          break;

        case -1:
        default:
          shallContinue = false;
          cout << "Terminating program..." << endl;
          continue; //jump to end of loop
          break;
      }

      //Create new buffer with correct size
      transmission_length = protocol->get_buffersize();
      char* buffer = new char[transmission_length]();
      
      protocol->serialize(buffer);
      if (sendall(socket_fd, buffer, transmission_length) == -1)
        {
          perror("sendall");
          printf("We only sent %d bytes because of the error!\n", transmission_length);
        }

      //Receive Server answere
      size = recvall(socket_fd, vec_receive_buffer);
      if (size > 0)
      {
        char *receiveBuffer = new char[vec_receive_buffer.size()];
        memset(receiveBuffer, 0, vec_receive_buffer.size());
        
        vec_to_bufNum(vec_receive_buffer, receiveBuffer, 2);
        vec_receive_buffer.clear();

        printf("%s\n", receiveBuffer);
      }
      else if (size == 0)
      {
        printf("Server closed remote socket\n");
        break;
      }
      else
      {
        perror("recv error");
        return EXIT_FAILURE;
      }

      delete buffer;
      delete protocol;

      usleep(2000000);//wait 2 seconds
      

    } 
  while (shallContinue == true);

  close (socket_fd);
  return EXIT_SUCCESS;
}
