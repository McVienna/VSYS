#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <vector>
#include <iostream>
#include <string>

#include "socketutility.h"

using namespace std;

void vec_to_buf(std::vector<char> &m_buffer, char * temp_buffer)
{
    for(unsigned int i = 0; i < m_buffer.size(); i++)
    {
        temp_buffer[i] = m_buffer[i];
    }
}

int sendall(int socketfd, char* buffer, unsigned int &len)
{
  unsigned int total = 0;
  int bytesleft = len;  // how many we have left to send
  int n;

  /*DEBUG: TEST PRINTING OF ARRAY 
  cout << "CLIENT BEFORE SEND" << endl;
  cout << ntohs(buffer[0] << 8 | buffer[1]) << endl;
  cout << ((int) (buffer[2] - '0')) << endl;

  for(unsigned int i = 3; i < len; i++)
  {
      cout << buffer[i] << endl;
  }
  */

  while(total < len)
    {
      n = send(socketfd, buffer+total, bytesleft, 0);
      if (n == -1)
        {
          break;
        }
      total += n;
      bytesleft -= n;
    }
  len = total;      // return number actually sent here
  return (n == -1) ? (-1) : (0); // return -1 on failure, 0 on success
 
}


/*
receive all Data from socket.
-1 error
 0  client closed connection
>0  size of data received.
*/
int recvall(int socketfd, std::vector<char> &storage_buffer)
{
  unsigned int total = 0;         // how many bytes we’ve received
  int size = 0;
  unsigned short packagesize = 0;
  char* buffer = new char[4];

  //first receive
  while(total < 2)
    {
      size = recv(socketfd, buffer+total, 2-total, 0);
      if (size > 0)
      {
          total += size;
      }
      else if (size == 0)
      {
          return 0;
      }
      else
      {
          perror("recv error");
          return -1;
      }
    }
  
  //get length of incoming package
  packagesize = buffer[0] << 8 | buffer[1];
  packagesize = ntohs(packagesize);

  //resize buffer
  char *temp = buffer;
  buffer     = new char[packagesize];
  strcpy(buffer, temp);
  delete temp;
    

  while(total < packagesize)
  {
      size = recv(socketfd, buffer+total, total-packagesize, 0);
      if (size > 0)
      {
          total += size;
      }
      else if (size == 0)
      {
          return 0;
      }
      else
      {
          perror("recv error");
          return -1;
      }
  }
    /*DEBUG
    cout << "SERVER AFTER RECIEVING" << endl;
    cout << ntohs(buffer[0] << 8 | buffer[1]) << endl;
    cout << ((buffer[2] - '0')) << endl;
    
    for(unsigned int i = 3; i < packagesize; i++)
      {
        cout << buffer[i] << endl;
      }
   */
  storage_buffer.resize(packagesize);
  for(unsigned int i = 0; i < packagesize; i++)
  {
      storage_buffer[i] = buffer[i];
  }

  delete buffer;
  return size; // return -1 on failure, 0 on success
}