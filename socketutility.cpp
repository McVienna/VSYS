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

*/
int recvall(int socketfd, char* buf, unsigned int &len)
{
  unsigned int total = 0;         // how many bytes we’ve recieved
  int bytesleft = len;  // how many we have left to recieve
  int size = 0;

  //first recieve
  while(size < 2)
    {
      size = recv(socketfd, buf+total, len, 0);
      if (size > 0)
        {
          total += size;
          bytesleft -=size;
        }
      else if (size == 0)
        {
          return 0;
        }
      else
        {
          perror("recv error");
          return EXIT_FAILURE;
        }
    }
  
  while(total < len)
    {
      size = recv(socketfd, buf+total, bytesleft, 0);
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


