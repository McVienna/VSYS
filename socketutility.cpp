#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <vector>

#include "socketutility.h"

void vec_to_buf(std::vector<char> &m_buffer, char * temp_buffer)
{
    for(unsigned int i = 0; i < m_buffer.size(); i++)
    {
        temp_buffer[i] = m_buffer[i];
    }
}

int sendall(int socketfd, char* buf, unsigned int &len)
{
  unsigned int total = 0;
  int bytesleft = len;  // how many we have left to send
  int n;

  while(total < len)
    {
      n = send(socketfd, buf+total, bytesleft, 0);
      if (n == -1)
        {
          break;
        }
      total += n;
      bytesleft -= n;
    }

  len = total;      // return number actually sent here
  return n==-1?-1:0; // return -1 on failure, 0 on success
 
}


/* recieve all Data from socket.
-1 error
 0  client closed connection
>0  size of data recieved.

*/
int recvall(int socketfd, char* &buffer)
{
  unsigned int total = 0;         // how many bytes we’ve recieved
  int size = 0;
  unsigned short packagesize = 0;

  //first recieve
  while(size < 2)
    {
      size = recv(socketfd, buffer+total, 4, 0);
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
  return size == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


/**/