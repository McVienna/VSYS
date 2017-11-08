/* myserver.c, adapted to c++ */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <experimental/filesystem>
#include <string>
#include <sstream>

#include <thread>
#include <mutex>

#include "socketutility.h"
#include "protocols.h"
#include "s_filehandler.h"
#include "ldaphandler.h"


#define BUF 2048

namespace fs = std::experimental::filesystem::v1;

using namespace std;

void buildProtocol(Protocol* &emptyProtocol, int protocolType, char* data); //set correct protocoll type
void prepareBuffer(char *&intoBuffer, char *&message, unsigned int transmission_length); //parse message into intoBuffer, ready for sendall. NOTE: Transmission_length = strlen(buffer) + 2

    int main(int argc, char **argv)
{

    std::vector<char> vec_receive_buffer;
    char* buffer = new char[BUF];
    unsigned int transmission_length =0;

    int server_socket_fd, client_socket_fd;
    socklen_t addrlen;

    int size;
    std::string _read;

    unsigned short port = 0;
    int portRangeCheck = 0;

    int protocolType;
    Protocol *received_Protocol = NULL;

    std::string _path;
    filehandler *general_filehandler = NULL;

    Ldap ldaphandler;

    if (argc != 3)
    {
        perror("Usage: ./MyServer <Mailpoolpath> <Port>");
        return EXIT_FAILURE;
    }

    istringstream portCheck(argv[2]);
    if (!portCheck >> port)
    {
        printf("Invalid Port!\n");
        exit(EXIT_FAILURE);
    }
    portRangeCheck = atoi(argv[2]);
    if ((portRangeCheck < 1024) || (portRangeCheck > 65535))
    {
        printf("Invalid Port Range!\n");
        exit(EXIT_FAILURE);
    }
    
    port = atoi(argv[2]);
    //Create Socket
    struct sockaddr_in address, cliaddress;
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket_fd, (struct sockaddr *) &address, sizeof(address)) != 0) {
        perror("bind error");
        return EXIT_FAILURE;
    }
    listen(server_socket_fd, 5);

    addrlen = sizeof(struct sockaddr_in);

//Main Program, runs until killed.
    _path = argv[1];

    general_filehandler = new filehandler(_path);

    while (1) { //Wait for Connection

        printf("Waiting for connections...\n");
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &cliaddress, &addrlen);

        if (client_socket_fd > 0) {
            printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));
            strcpy(buffer, "Welcome to TWMailer, Please enter your command:\n");

            send(client_socket_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUF-1);
        }

        //Set up Ldap
        LDAP *myldap;
        int locktime;
        unsigned int clientAddress = cliaddress.sin_addr.s_addr;
        bool loggedIn = false;
        std::string username;

        //setup LDAP connection
        if (!ldaphandler.init(myldap))
        {
            perror("ERR: ldap_init failed! ldap Server not reachable\n");
            return EXIT_FAILURE;
        }
        printf("connected to LDAP server %s on port %d\n", LDAP_HOST, LDAP_PORT);

        //Communication with Client
        do {
            //Recieve message
            vec_receive_buffer.clear();
            size = recvall(client_socket_fd, vec_receive_buffer);
            if (size > 0)
            {
                //in case of send_prot a dynamically sized Buffer is allocated
                char* receiveBuffer = new char[1];
                char* temp = receiveBuffer;

                receiveBuffer  = new char[vec_receive_buffer.size()];
                delete temp;
                vec_to_buf(vec_receive_buffer, receiveBuffer);

                //get protocol type. Check for error (-1)
                protocolType = get_protocol(receiveBuffer);
                if(protocolType < 0)
                {
                    perror("ERR: Protocol transmission went wrong!");
                    return EXIT_FAILURE;
                }
                buildProtocol(received_Protocol, protocolType, receiveBuffer);
                delete receiveBuffer;

                if(!loggedIn && protocolType != 4)
                {
                    protocolType = -1;
                    strcpy (buffer, "ERR: Not logged in!\n");
                }

                switch(protocolType)
                {
                    case 0: //SEND
                    {
                        bool succ = false;


                        Send_prot *send_prot = static_cast<Send_prot *>(received_Protocol);

                        succ = general_filehandler->handle_message(send_prot);

                        if(succ == true)
                        {
                            strcpy(buffer, "OK");
                            delete send_prot;
                            break;
                        }
                        else
                        {
                            strcpy(buffer, "ERR");
                            delete send_prot;
                            break;
                        }
                    }

                    case 1: //LIST
                    {

                        List_prot * to_list = static_cast<List_prot *>(received_Protocol);

                        std::string _list = general_filehandler->list_mails(to_list);

                        const char * list_mails = _list.c_str();

                        strcpy(buffer, list_mails);

                        break;
                    }

                    case 2: //READ
                    {
                        Read_prot *read_msg = static_cast<Read_prot *>(received_Protocol);

                        /*KIND OF STRANGE BEHAVIOUR OCCURED HERE*/

                        _read = general_filehandler->read_msg(read_msg);

                        if(!(_read.empty()))
                        {

                            const char * send_read = _read.c_str();

                            strcpy(buffer, send_read);
                        }

                        break;
                    }

                    case 3: //DEL
                    {
                        int check = 0;

                        Delete_prot *delete_msg = static_cast<Delete_prot *>(received_Protocol);

                        check = general_filehandler->delete_message(delete_msg);

                        if(check == -1)
                        {
                            strcpy(buffer, "ERR");
                            break;
                        }
                        else
                        {
                            strcpy(buffer, "OK");
                            break;
                        }
                        break;

                    }   
                    case 4: //LOGIN
                    {
                        Login_prot* login_prot = static_cast<Login_prot* >(received_Protocol);
                        int rc = ldaphandler.login(myldap, login_prot, clientAddress, locktime, loggedIn, username);
                        delete login_prot;

                        if (rc == LDAP_SUCCESS)
                        {
                            sprintf(buffer, "OK\n");
                        }
                        else if (rc == -1)
                        {
                            sprintf(buffer, "Too many attempts...pls wait another %d Minutes", (locktime / 60));
                        }
                        else if (rc == -2)
                        {
                            sprintf(buffer, "You are already logged in %s", username.c_str());
                        }
                        else if (rc != LDAP_SUCCESS)
                        {
                            sprintf(buffer, "ERR: %s\n", ldap_err2string(rc));
                        }

                        //TODO: SEND MESSAGE TO CLIENT
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else if (size == 0)
            {
                printf("Client closed remote socket\n");
                break;
            }
            else
            {
                perror("recv error");
                return EXIT_FAILURE;
            }

            //Server Answere to client
            transmission_length = (strlen(buffer) + 2);

            char* sendBuffer = new char[transmission_length];
            memset(sendBuffer, 0, transmission_length);

            prepareBuffer(sendBuffer, buffer, transmission_length); //prepare for sendall
            if (sendall(client_socket_fd, sendBuffer, transmission_length))
            {
                perror("ERR: SENDALL");
                printf("We only sent %d bytes because of the error!\n", transmission_length);
            }
            memset(buffer, 0, BUF-1);

        } while (1);
        close(client_socket_fd);
    }
    close(server_socket_fd);
    return EXIT_SUCCESS;
}

void buildProtocol(Protocol* &emptyProtocol, int protocolType, char* data) {
      /*
      -1: error
      0: Send_prot
      1: List_prot
      2: Read_prot
      3: Delete_prot
      */
      switch(protocolType)
      {
        case 0:
          cout << "RECIEVED SEND REQUEST" << endl;
          emptyProtocol = new Send_prot(data);
          break;

        case 1:
          cout << "RECIEVED LIST REQUEST" << endl;
          emptyProtocol = new List_prot(data);
          break;

        case 2:
          cout << "RECIEVED READ REQUEST" << endl;
          emptyProtocol = new Read_prot(data);
          break;

        case 3:
          cout << "RECIEVED DELETE REQUEST" << endl;
          emptyProtocol = new Delete_prot(data);
          break;

        case 4:
            cout << "RECIEVED LOGIN REQUEST" << endl;
            emptyProtocol = new Login_prot(data);
            break;
      }
}

//sets tranmission length into buffer 
void prepareBuffer(char* &intoBuffer, char* &message, unsigned int transmission_length) {

    intoBuffer[0] = transmission_length & 0xFF;
    intoBuffer[1] = (transmission_length >> 8) & 0xFF;

    for(unsigned int i = 2; i < transmission_length; i++)
    {
        intoBuffer[i] = message[i-2];
    }
}
