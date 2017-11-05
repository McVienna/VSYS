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

#include <thread>
#include <mutex>

#include "socketutility.h"
#include "protocols.h"
#include "s_filehandler.h"
#include "ldaphandler.h"

#define BUF 1024
#define PORT 6554
///Port hardcoded for comfort of testing ^^

namespace fs = std::experimental::filesystem::v1;

using namespace std;

void buildProtocol(Protocol* &emptyProtocol, int protocolType, char* data);

int main(int argc, char **argv) {

    std::vector<char> m_buffer;
    char* buffer = new char[BUF];
    int server_socket_fd, client_socket_fd;
    socklen_t addrlen;

    std::string message;
    int size;

    int protocolType;
    Protocol *received_Protocol = NULL;

    std::string _path;
    filehandler *general_filehandler = NULL;

    Ldap ldaphandler;


    //Create Socket
    struct sockaddr_in address, cliaddress;
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket_fd, (struct sockaddr *) &address, sizeof(address)) != 0) {
        perror("bind error");
        return EXIT_FAILURE;
    }
    listen(server_socket_fd, 5);

    addrlen = sizeof(struct sockaddr_in);

//Main Program, runs until killed.
    if (argc == 2) {

        _path = argv[1];

        general_filehandler = new filehandler(_path);

        while (1) { //Wait for Connection

            printf("Waiting for connections...\n");
            client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &cliaddress, &addrlen);

            if (client_socket_fd > 0) {
                printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));
                message = "Welcome to TWMailer, Please enter your command:\n";
                std::copy(message.begin(), message.end(), std::back_inserter(m_buffer));
                
                char * temp_buffer = buffer;
                buffer = new char[m_buffer.size()];
                delete temp_buffer;
                vec_to_buf(m_buffer, buffer);

                send(client_socket_fd, buffer, strlen(buffer), 0);
                m_buffer.clear();

            }
            //Communication with Client
            do {

                LDAP *myldap;
                 /* LDAP resource handle */

                /* setup LDAP connection */
                if (!ldaphandler.init(myldap))
                {
                    perror("ERR: ldap_init failed! ldap Server not reachableP");
                    return EXIT_FAILURE;
                }
                printf("connected to LDAP server %s on port %d\n", LDAP_HOST, LDAP_PORT);


                //Recieve message
                m_buffer.clear();
                size = recvall(client_socket_fd, m_buffer);
                if (size > 0)
                {
                    char *temp = buffer;
                    buffer     = new char[m_buffer.size()];
                    delete temp;
                    vec_to_buf(m_buffer, buffer);

                    //get protocol type. Check for error (-1)
                    protocolType = get_protocol(buffer);
                    if(protocolType < 0)
                    {
                        perror("ERR: Protocol transmission went wrong!");
                        return EXIT_FAILURE;
                    }
                    buildProtocol(received_Protocol, protocolType, buffer);
                    
                    switch(protocolType)
                    {
                        case 0: //SEND
                        {
                            Send_prot *send_prot = static_cast<Send_prot *>(received_Protocol);
                            general_filehandler->handle_message(send_prot);
                            delete send_prot;

                            break;
                        }

                        case 1: //LIST
                        {    
                            break;
                        }

                        case 2: //READ
                        {
                            break;
                        }

                        case 3: //DEL
                        {
                            break;
                        }   
                        case 4: //LOGIN
                        {
                            Login_prot* login_prot = static_cast<Login_prot* >(received_Protocol);
                            int rc = ldaphandler.login(myldap, login_prot);
                            delete login_prot;

                            if (rc != LDAP_SUCCESS)
                            {
                                fprintf(stderr, "LDAP error: %s\n", ldap_err2string(rc));
                                return EXIT_FAILURE;
                            }
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

                delete received_Protocol;

            } while (1);//WHILE BEDINGUNG ANPASSEN AN EINGABE #later #überhaupt notwendig?
            close(client_socket_fd);
        }
        close(server_socket_fd);
        return EXIT_SUCCESS;
    } else if (argc < 2) {
        perror("Usage: ./MyServer <Mailpoolpath> <Port>");
        return EXIT_FAILURE;
    } else {
        return EXIT_FAILURE;
    }
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