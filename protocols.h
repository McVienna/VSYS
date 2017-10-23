#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <iostream>
#include <string>

const int PROTOCOL_HEADER_SIZE = 3;

/************************UTILITY FUNCTIONS************************/

/*
returns int depending on Protocol of 'serialized_data'
-1: error
 0: Send_prot
 1: List_prot
 2: Read_prot
 3: Delete_prot
*/
int get_protocol(char* received_data); 

//sets a string with limit being max amount of letters.
void set_with_limit(std::string name, std::string &toSet, unsigned int limit);

//function to read Message typed by user into string.
void typeMessage(std::string &message);

//Given a string, enter each digit into a char Array serializing it. "setPosition" gives starting Index within the Array.
void serialize_string(char* serialized_Array, int setPosition, std::string toSerialize, int maxStringLength);

//Given an Array, and a starting Indet('setPosition'), derseialize into String, fpr max of string Length
void deserialize_string(char* serialized_Array, int setPosition, std::string &intoString, int maxStringLength);



/************************PROTOCOLS************************/

//Protocol for sending a new email
class Send_prot{
  public:
    Send_prot();
    Send_prot(char* received_data); //used to build Object after recieving Data
    ~Send_prot();

    void serialize(char* serialized_data); //serializes all data into char Array.
    int get_buffersize(); //returns size needed for buffer to contain all data.
    std::string return_sender();

  private:
    std::string sender;
    std::string receiver;
    std::string subject;
    std::string message;

    const int SENDER_SIZE   = 8;
    const int RECEIVER_SIZE = 8;
    const int SUBJECT_SIZE  = 80;
};

//Protocol for listing all messages of a user stored on the server.
class List_prot{
  public:
    List_prot();
    List_prot(char* received_data); //used to build Object after recieving Data
    ~List_prot();
  private:
    char* serialize(); //returns c_string ready to send.

    std::string username;
    
};

//Protocol for reading a message specified by the user.
class Read_prot{
  public:
    Read_prot();
    Read_prot(char* received_data); //used to build Object after recieving Data
    ~Read_prot();

  private:
    char* serialize(); //returns c_string ready to send.

    std::string username;
    std::string message_nr;

};

//Protocol for deleting a message specified by the user.
class Delete_prot {
  public:
    Delete_prot();
    Delete_prot(char* received_data); //used to build Object after recieving Data
    ~Delete_prot();

  private:
    char* serialize(); //returns c_string ready to send.

    std::string username;
    std::string message_nr;

};
    

#endif /* PROTOCOLS_H*/