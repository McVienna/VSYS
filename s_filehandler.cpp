#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
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

void filehandler::handle_message(Send_prot * &insta_message)
{
    std::string user_path;
    std::string time;
    std::stringstream _cast;

    std::time_t result = std::time(nullptr);

    _cast << result;

    time = _cast.str();

    user_path = this->path + "/" + insta_message->return_sender();

    std::string msg_name = user_path + "/" + time + ".txt";

    if(!(fs::is_directory(user_path)))
    {
        fs::create_directories(user_path);

        std::ofstream message_file (msg_name);

        message_file << "#" << insta_message->return_sender() << "#" << '\n';

        message_file << "#" << insta_message->return_receiver() << "#"  << '\n';

        message_file << "#" << insta_message->return_subject() << "#"  << '\n';

        message_file << "#" << insta_message->return_message() << "#"  << '\n';


    }
    else
    {
        std::ofstream message_file (msg_name);

        message_file << "#" << insta_message->return_sender() << "#" << '\n';

        message_file << "#" << insta_message->return_receiver() << "#"  << '\n';

        message_file << "#" << insta_message->return_subject() << "#"  << '\n';

        message_file << "#" << insta_message->return_message() << "#"  << '\n';
    }


}

filehandler::~filehandler(){

}


std::string filehandler::return_path()
{
    return this->path;
}

void filehandler::write_data_to_file(std::ofstream msg_file, Send_prot * rec_msg)
{
    msg_file << rec_msg->return_sender();
    msg_file.close();
}


