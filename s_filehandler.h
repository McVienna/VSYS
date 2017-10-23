#ifndef S_FILEHANDLER_H
#define S_FILEHANDLER_H
#include "protocols.h"

#include <string>

class filehandler{
private:
    std::string path;
    std::string user;

public:
    filehandler(std::string);
    ~filehandler();
    void create_usr_dir(Send_prot *);
    std::string return_path();

};


#endif /* S_FILEHANDLER.H*/