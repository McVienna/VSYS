#ifndef SOCKETUTILITY_H
#define SOCKETUTILITY_H

//Outsourced Utilility functions for Socket communication
#include <vector>



void vec_to_buf(std::vector<char> &, char *);
void vec_to_bufNum(std::vector<char> &, char *, int );//start copy from vector at index given.


//Recieve and send ALL data.
int sendall (int , char* , unsigned int &);
int recvall (int , std::vector<char> &);

int sendAnswere(bool succeeded, int socketfd); //sends ERR/OK answere based on result.
int recvAnswere(int socketfd);                 //receive ERR/OK answere and print message based on result.

#endif //SOCKETUTILITY_H INCLUDED