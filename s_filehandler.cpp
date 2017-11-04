#include <iostream>
#include "s_filehandler.h"
#include "protocols.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem::v1;


//Protocol * emptyProtocol = NULL;

filehandler::filehandler(std::string m_path){

    //Send_prot * insta_message = static_cast<Send_prot*>(rec_prot);

    this->path = m_path;

    std::cout << "Hello im your filehandler for today!" << std::endl;

    if(!(fs::is_directory(this->path)))
    {
        fs::create_directories(this->path);
    }
    else
    {

    }

    //std::cout << insta_message->return_sender();

}

void filehandler::create_user(Protocol* &rec_prot)
{
    std::cout << "Hello i should create a user directory!" << std::endl;

    Send_prot * insta_message = static_cast<Send_prot*>(rec_prot);

    std::cout << "Your user is -> "<< insta_message->return_sender() << std::endl;
}

filehandler::~filehandler(){

}


std::string filehandler::return_path()
{
    return this->path;
}


