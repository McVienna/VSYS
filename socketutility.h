#ifndef SOCKETUTILITY_H
#define SOCKETUTILITY_H


#include <vector>

void vec_to_buf(std::vector<char> &, char *);
int sendall (int , char* , unsigned int &);
int recvall (int , std::vector<char> &);

#endif //SOCKETUTILITY_H INCLUDED