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
    void write_data_to_file(std::ofstream, Send_prot*);
    ~filehandler();
    std::string return_path();

};


#endif /* S_FILEHANDLER.H*/