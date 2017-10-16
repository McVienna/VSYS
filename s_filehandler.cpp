#include <iostream>
#include "s_filehandler.h"
#include <boost/filesystem.hpp>


filehandler::filehandler(std::string m_path){
    this->path = m_path;

    if(!(boost::filesystem::is_directory(m_path)))
    {
        std::cout << "Currently not a Directory!" << std::endl;
        std::cout << "Creating Directory " << path << std::endl;
        boost::filesystem::create_directories(path);
    }
    else
    {
        std::cout << "Starting Server in " << path << std::endl;
    }
}

filehandler::~filehandler(){

}


