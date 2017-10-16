#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <iostream>

/*
returns int depending on Protocol of 'serialized_data'
-1: error
 0: Send_prot
 1: List_prot
 2: Read_prot
 3: Delete_prot
*/
int get_protocol(char* serialized_data); 


//Protocol for sending a new email
class Send_prot{
  public:
    Send_prot();
    Send_prot(char* serialized_data); //used to build Object after recieving Data
    ~Send_prot();

  private:
    void set_data(); //called when constructing the element
    char* serialize(); //returns c_string ready to send.

    std::string sender;
    std::string reciever;
    std::string subject;
    std::string messege;
};

//Protocol for listing all messages of a user stored on the server.
class List_prot{
  public:
    List_prot();
    List_prot(char* serialized_data); //used to build Object after recieving Data
    ~List_prot();
  private:
    void set_data(); //called when constructing the element
    char* serialize(); //returns c_string ready to send.

    std::string username;
    
};

//Protocol for reading a message specified by the user.
class Read_prot{
  public:
    Read_prot();
    Read_prot(char* serialized_data); //used to build Object after recieving Data
    ~Read_prot();

  private:
    void set_data(); //called when constructing the element
    char* serialize(); //returns c_string ready to send.

    std::string username;
    std::string message_nr;

};

//Protocol for deleting a message specified by the user.
class Delete_prot {
  public:
    Delete_prot();
    Delete_prot(char* serialized_data); //used to build Object after recieving Data
    ~Delete_prot();

  private:
    void set_data(); //called when constructing the element
    char* serialize(); //returns c_string ready to send.

    std::string username;
    std::string message_nr;

};
    

#endif /* PROTOCOLS_H*/