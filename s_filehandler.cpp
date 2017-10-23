#include <iostream>
#include "s_filehandler.h"
#include "protocols.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;


filehandler::filehandler(std::string m_path){

    this->path = m_path;

    if(!(fs::is_directory(this->path)))
    {
        fs::create_directories(this->path);
    }
    else
    {

    }

}

filehandler::~filehandler(){

}

void filehandler::create_usr_dir(Send_prot * instanciate_message) {
    ///should create subdirectory if message received -> sub_dir name = username
    std::string user_path = this->path + "/" + this->user;
    this->user = instanciate_message->return_sender();

    if(!(fs::is_directory(user_path)))
    {
        fs::create_directories(user_path);
        this->path = user_path;
    }
    else
    {
        this->path = user_path;
    }
}

std::string filehandler::return_path()
{
    return this->path;
}

