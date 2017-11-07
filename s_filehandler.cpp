#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include<sys/types.h>
#include<dirent.h>
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

        message_file << insta_message->return_sender() << "#" << '\n';

        message_file << insta_message->return_subject() << "#"  << '\n';

        message_file << insta_message->return_message() << "#"  << '\n';

        return true;


    }
    else if(fs::is_directory(user_path))
    {
        std::ofstream message_file (msg_name);

        message_file << insta_message->return_sender() << "#" << '\n';

        message_file << insta_message->return_subject() << "#"  << '\n';

        message_file << insta_message->return_message() << "#"  << '\n';

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

std::string filehandler::read_msg(Read_prot* &to_read)
{
    std::string path = this->path + "/" + to_read->return_username() + "/" + to_read->return_message_nr() + ".txt";
    std::string msg;
    std::string _temp;
    std::string topic;

    std::ifstream input_file(path);

    if(input_file.is_open())
    {
        for(int i = 0; i < 3; i++)
        {

            switch(i)
            {
                case 0:
                {
                    topic ="\nsender --> \n";
                    getline(input_file, _temp, '#');
                    _temp = topic + _temp + "\n\n";
                    break;
                }
                case 1:
                {
                    topic = "subject --> ";
                    getline(input_file, _temp, '#');
                    _temp = topic + _temp + "\n\n";
                    break;
                }
                case 2:
                {
                    topic = "message -->";
                    getline(input_file, _temp, '#');
                    _temp = topic + _temp + "\n\n";
                    break;
                }
            }

            for(int i = 0; i < _temp.size(); i++)
            {

                    msg.push_back(_temp[i]);

            }
        }
    }
    else
    {
      return "ERR";
    }

    input_file.close();

    return msg;


}

//Function gets number of files with ".txt" extension out of given directory


std::string filehandler::list_mails(List_prot *&to_list) {

    std::string directory = this->path + "/" + to_list->return_usr();
    std::string actual_file;
    std::string my_list;
    std::string _temp;


    std::cout << directory << std::endl;

    std::string ext = ".txt";

    fs::path Path(directory);

    int Nb_ext = 0;
    fs::directory_iterator end_iter; // Default constructor for an iterator is the end iterator

    for (fs::directory_iterator iter(Path); iter != end_iter; ++iter) {

        if (iter->path().extension() == ext)
        {
            actual_file = iter->path().filename();

            std::string _file = this->path + "/" + to_list->return_usr() + "/" + actual_file;

            std::ifstream in_file(_file);

            if(in_file.is_open())
            {
                if(in_file.is_open())
                {

                    my_list.push_back('-');

                    for(int i = 0; i < _file.size(); i++)
                    {
                        my_list.push_back(_file[i]);
                    }

                    my_list.push_back('\n');

                    for(int i = 0; i < 2; i++)
                    {


                        switch(i) {
                            case 0: {
                                getline(in_file, _temp, '#');
                                _temp = _temp + " -->";

                                std::cout << _temp << std::endl;
                                break;
                            }
                            case 1: {
                                getline(in_file, _temp, '#');
                                std::cout << _temp << std::endl;
                                break;
                            }
                        }

                        for(int i = 0; i < _temp.size(); i++)
                        {

                            if(_temp[i] != '\n')
                            {
                                my_list.push_back(_temp[i]);
                            }

                        }

                        my_list.push_back('\n');
                    }
                }

            }
        }

    }

    std::cout << my_list;

    return my_list;
}