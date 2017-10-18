#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "socketutility.h"

int sendall(int socketfd, char* buf, unsigned int &len)
{
  unsigned int total = 0;         // how many bytes we’ve sent
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
-0  client closed connection
>0  size of data recieved.


int recvall(int socketfd, char* buf, unsigned int &len)
{
  unsigned int total = 0;         // how many bytes we’ve recieved
  int size = 0;
  short packagesize = 0;

  //first recieve
  while(size < 2)
    {
      size = recv(socketfd, buf+total, len, 0);
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
  
    packagesize = buf[0] << 8 | buf[1];
    packagesize = ntohs(packagesize);
    size = 0;

  if(sizeof(buf) < packagesize)
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
  
  while(total < packagesize)
    {
      size = recv(socketfd, buf+total, packagesize, 0);
      if (size == -1)
        {
          break;
        }
      total += size;
      bytesleft -= size;
    }

  len = total;      // return number actually sent here
  return size ==-1 ? -1 : 0; // return -1 on failure, 0 on success
}


*/