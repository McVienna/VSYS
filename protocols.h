#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <iostream>

//Protocol for sending a new email
class send_prot() {
  public:
    send_prot();
    ~send_prot();

  private:
    set_data(); //called when constructing the element

    std::string sender;
    std::string reciever;
    std::string subject;
    std::string messege;
};
//Protocol for listing all messages of a user stored on the server.
class list_prot() {
  public:
    send_list();
    ~send_list();

  private:
    set_data(); //called when constructing the element

    std::string username;
    
};

//Protocol for reading a message specified by the user.
class read_prot() {
  public:
    read_prot();
    ~read_prot();

  private:
    set_data(); //called when constructing the element

};

//Protocol for deleting a message specified by the user.
class delete_prot() {
  public:
    delete_prot();
    ~delete_prot();

  private:
    set_data(); //called when constructing the element

};
    

#endif /* PROTOCOLS_H*/