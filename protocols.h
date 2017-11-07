#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <iostream>
#include <string>

/************************UTILITY FUNCTIONS************************/
int get_protocol(char* received_data); 

/************************PROTOCOLS************************/

class Protocol {
  public:

    Protocol();
    virtual ~Protocol();

    virtual void serialize(char* serialized_data) = 0; //serializes all data into char Array.
    virtual int  get_buffersize() = 0; //returns size needed for buffer to contain all data.

  protected:
    const int PROTOCOL_HEADER_SIZE = 3;

    /*
    returns int depending on Protocol of 'serialized_data'
    -1: error
    0: Send_prot
    1: List_prot
    2: Read_prot
    3: Delete_prot
    */
    
    //sets a string with limit being max amount of letters.
    void set_with_limit(std::string name, std::string &toSet, unsigned int limit);

    //Given a string, enter each digit into a char Array serializing it. "setPosition" gives starting Index within the Array.
    void serialize_string(char* serialized_Array, int setPosition, std::string toSerialize, int maxStringLength);

    //Given an Array, and a starting Indet('setPosition'), derseialize into String, for max of string Length
    int deserialize_string(char* serialized_Array, int setPosition, std::string &intoString, int maxStringLength);

};

//Protocol for sending a new email
class Send_prot : public Protocol {
  public:
    Send_prot();
    Send_prot(char* received_data); //used to build Object after recieving Data
    ~Send_prot();

    void serialize(char* serialized_data); //serializes all data into char Array.
    int get_buffersize(); //returns size needed for buffer to contain all data.

    std::string return_sender();
    std::string return_receiver();
    std::string return_subject();
    std::string return_message();

  private:
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string message;

    const int SENDER_SIZE   = 8;
    const int RECEIVER_SIZE = 8;
    const int SUBJECT_SIZE  = 80;

    //function to read Message typed by user into string.
    void typeMessage(std::string &message);
};

//Protocol for listing all messages of a user stored on the server.
class List_prot : public Protocol {
  public:
    List_prot();
    List_prot(char* received_data); //used to build Object after recieving Data
    ~List_prot();

    void serialize(char* serialized_data); //serializes all data into char Array.
    int get_buffersize(); //returns size needed for buffer to contain all data.


  private:
    std::string username;

    const int USERNAME_SIZE = 8;
    
};

//Protocol for reading a message specified by the user.
class Read_prot : public Protocol {
  public:
    Read_prot();
    Read_prot(char* received_data); //used to build Object after recieving Data
    ~Read_prot();

    void serialize(char* serialized_data); //serializes all data into char Array.
    int get_buffersize(); //returns size needed for buffer to contain all data.

  private:
    std::string username;
    std::string message_nr;

    const int USERNAME_SIZE = 8;
};

//Protocol for deleting a message specified by the user.
class Delete_prot  : public Protocol{
  public:
    Delete_prot();
    Delete_prot(char* received_data); //used to build Object after recieving Data
    ~Delete_prot();

    void serialize(char* serialized_data); //serializes all data into char Array.
    int get_buffersize(); //returns size needed for buffer to contain all data.

  private:
    std::string username;
    std::string message_nr;

    const int USERNAME_SIZE = 8;
};

//Protocol for deleting a message specified by the user.
class Login_prot : public Protocol
{
public:
  Login_prot();
  Login_prot(char *received_data); //used to build Object after recieving Data
  ~Login_prot();

  void serialize(char *serialized_data); //serializes all data into char Array.
  int get_buffersize();                  //returns size needed for buffer to contain all data.

  std::string get_username();
  std::string get_password();

private:
  std::string username;
  std::string password;

  const int USERNAME_SIZE = 8;
  const int PASSWORD_SIZE = 64;
};
#endif /* PROTOCOLS_H*/