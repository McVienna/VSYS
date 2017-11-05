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
    void handle_message(Protocol*&);
    ~filehandler();
    std::string return_path();

};


#endif /* S_FILEHANDLER.H*/