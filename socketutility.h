#ifndef SOCKETUTILITY_H
#define SOCKETUTILITY_H


#include <vector>

void vec_to_buf(std::vector<char> &, char *);
int sendall (int socketfd, char* buf, unsigned int &len);
int recvall (int socketfd, std::vector<char> &);

#endif //SOCKETUTILITY_H INCLUDED