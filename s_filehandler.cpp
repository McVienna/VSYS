#include <iostream>
#include <cstdio>
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

bool filehandler::handle_message(Send_prot * &insta_message)
{
    std::string receiver_path;
    std::string sender_path;
    std::string user_path;
    std::string time;
    std::stringstream _cast;

    std::time_t result = std::time(nullptr);

    _cast << result;

    time = _cast.str();

    receiver_path = this->path + "/" + insta_message->return_receiver();

    std::string msg_name = receiver_path + "/" + time + ".txt";

    if(!(fs::is_directory(user_path)))
    {
        fs::create_directories(receiver_path);

        std::ofstream message_file (msg_name);

        message_file << "#" << insta_message->return_sender() << "#" << '\n';

        message_file << "#" << insta_message->return_receiver() << "#"  << '\n';

        message_file << "#" << insta_message->return_subject() << "#"  << '\n';

        message_file << "#" << insta_message->return_message() << "#"  << '\n';

        return true;


    }
    else if(fs::is_directory(user_path))
    {
        std::ofstream message_file (msg_name);

        message_file << "#" << insta_message->return_sender() << "#" << '\n';

        message_file << "#" << insta_message->return_receiver() << "#"  << '\n';

        message_file << "#" << insta_message->return_subject() << "#"  << '\n';

        message_file << "#" << insta_message->return_message() << "#"  << '\n';

        return true;
    }
    else
    {
        return false;
    }


}

filehandler::~filehandler(){

}


std::string filehandler::return_path()
{
    return this->path;
}


///FUNCTION REMOVES A MESSAGE WITH GIVEN INPUT BY A USER

int filehandler::delete_message(Delete_prot* &delete_this)
{
    std::string delete_path = this->path + "/" + delete_this->return_usr();
    std::string to_delete = delete_path + "/" + delete_this->return_msg_nr() + ".txt";
    char* _del = new char[to_delete.size() + 1];
    std::copy(to_delete.begin(), to_delete.end(), _del);

    _del[to_delete.size()] = '\0';

    if(!(fs::is_directory(delete_path)))
    {
        std::cout << "An Error occured: seems like this directory doesnt exist!" << std::endl;
        return -1;
    }
    else
    {
        remove(_del);
        std::cout << "Message deleted successfully" << std::endl;
        return 1;
    }


}

/*FUNCTIONS FOR LIST AND READ ARE OUTCUTTET*/
/*MERGED IN THE MORNING CUZ DEAD*/


