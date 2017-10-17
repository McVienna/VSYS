#include <iostream>
#include "s_filehandler.h"
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

void filehandler::create_usr_dir(std::string usr, std::string path) {
    ///should create subdirectory if message recieved -> sub_dir name = username
}


