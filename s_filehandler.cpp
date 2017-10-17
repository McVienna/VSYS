#include <iostream>
#include "s_filehandler.h"
#include <boost/filesystem.hpp>


filehandler::filehandler(std::string m_path){
    this->path = m_path;

    if(!(boost::filesystem::is_directory(m_path)))
    {
        boost::filesystem::create_directories(path);
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


