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

    bool handle_message(Send_prot*&);
    int delete_message(Delete_prot*&);
    std::string read_msg(Read_prot*&);

    ~filehandler();
    std::string return_path();
};


#endif /* S_FILEHANDLER.H*/

/*FUNCTIONS FOR LIST AND READ ARE OUTCUTTET*/
/*MERGED IN THE MORNING CUZ DEAD*/