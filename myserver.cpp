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


#define BUF 1024

namespace fs = std::experimental::filesystem::v1;

using namespace std;

void buildProtocol(Protocol* &emptyProtocol, int protocolType, char* data);

int main(int argc, char **argv) {

    std::vector<char> m_buffer;
    char* buffer = new char[BUF];
    int server_socket_fd, client_socket_fd;
    socklen_t addrlen;

    int size;

    unsigned short port = 0;

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
    port = atoi(argv[2]);
    if (port < 1024 || port > 65535)
    {
        printf("Invalid Port Range!\n");
        exit(EXIT_FAILURE);
    }
    cout << port << endl;

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
        unsigned long clientAddress = cliaddress.sin_addr.s_addr;

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
            m_buffer.clear();
            size = recvall(client_socket_fd, m_buffer);
            if (size > 0)
            {
                //in case of send_prot a dynamically sized Buffer is allocated
                char* receiveBuffer = new char[1];
                char* temp = receiveBuffer;

                receiveBuffer  = new char[m_buffer.size()];
                delete temp;
                vec_to_buf(m_buffer, receiveBuffer);

                //get protocol type. Check for error (-1)
                protocolType = get_protocol(receiveBuffer);
                if(protocolType < 0)
                {
                    perror("ERR: Protocol transmission went wrong!");
                    return EXIT_FAILURE;
                }
                buildProtocol(received_Protocol, protocolType, receiveBuffer);
                delete receiveBuffer;

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
                        strcpy(buffer, "MyList answere!");

                        break;
                    }

                    case 2: //READ
                    {
                        strcpy(buffer, "MyList answere!");
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
                        int rc = ldaphandler.login(myldap, login_prot, clientAddress);
                        delete login_prot;
                        if(rc == -1)
                        {
                            strcpy(buffer, "Too many attempts...pls wait another");
                            printf("%s\n", buffer);
                        }
                        else if (rc != LDAP_SUCCESS)
                        {
                            sprintf(buffer, "LDAP error: %s\n", ldap_err2string(rc));
                            printf("%s\n", buffer);
                        }
                        //TODO: SEND MESSAGE TO CLIENT
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

        } while (1);//WHILE BEDINGUNG ANPASSEN AN EINGABE #later #überhaupt notwendig?
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