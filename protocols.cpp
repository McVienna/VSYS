#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "protocols.h"


using namespace std;

/************************Utility Functions************************/
/*
returns int depending on Protocol of 'serialized_data'
-1: error
 0: Send_prot
 1: List_prot
 2: Read_prot
 3: Delete_prot
*/
int get_protocol(char* received_data) {
    //check if received_data contains valid values.
    if(received_data[2] == '\0')
    {
        return -1;
    }

    return ((int) (received_data[2] - '0'));
}

/************************BASE PROTOCOL************************/
Protocol::Protocol() {
}

Protocol::~Protocol() {
}

//sets a string with limit being max amount of letters.
void Protocol::set_with_limit(std::string name, std::string &toSet, unsigned int limit) {
    int loop = 0;

    do
    {
        if(loop > 0)
        {
            cout << "Invalid length of argument! Watch the limit in parantheses!" << endl << endl;
        }
        cout << name << "(" << limit << "): ";
        getline(std::cin, toSet);

        loop++;
    }
    while(toSet.size() < 1 || toSet.size() > limit);
}

//Given a string, enter each digit into a char Array serializing it. "setPosition" gives starting Index within the Array.
void Protocol::serialize_string(char* serialized_Array, int setPosition, std::string toSerialize, int maxStringLength) {

    for(unsigned int i = 0; i < (unsigned int) maxStringLength; i++)
    {
        if(toSerialize.size() > i) serialized_Array[setPosition+i] = toSerialize[i];
        else                  break;
    }
}

//Given an Array, and a starting Indet('setPosition'), derseialize into String, for max of string Length
int Protocol::deserialize_string(char* serialized_Array, int setPosition, std::string &intoString, int maxStringLength) {

    unsigned int i = 0;

    for(i = 0; i < (unsigned int) maxStringLength; i++)
    {
        if(serialized_Array[setPosition+i] != 0)
        {
            intoString[i] = serialized_Array[setPosition+i];

        }
        else
        {
            return i;
        }
    }

    return i;
}







/************************SEND PROTOCOL************************/
Send_prot::Send_prot(std::string ldapUsername)
{
    //fill in strings of the object
    this->sender = ldapUsername;
    set_with_limit("Receiver", this->receiver, RECEIVER_SIZE);
    set_with_limit("Subject", this->subject, SUBJECT_SIZE);
    typeMessage(this->message);
}

//Build Object with given serialized data. Probably only used by Server, receiving the data from client.
Send_prot::Send_prot(char* received_data) {

    int arrayPos = 0;
    int messageSize = 0;
    int stringSize;

    //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
    short length = received_data[0] << 8 | received_data[1];
    length = ntohs(length);
    messageSize = (length - (PROTOCOL_HEADER_SIZE + SENDER_SIZE + RECEIVER_SIZE + SUBJECT_SIZE)); //Subtract fixed constants to determine message length.

    arrayPos += PROTOCOL_HEADER_SIZE;

    //first resize strings for maximum possible size
    this->sender.resize(SENDER_SIZE);
    this->receiver.resize(RECEIVER_SIZE);
    this->subject.resize(SUBJECT_SIZE);
    this->message.resize(messageSize);

    //Deserialize string from buffer and resize to correct size to avoid trailing zeros
    stringSize = deserialize_string(received_data, arrayPos, this->sender, SENDER_SIZE);
    arrayPos += SENDER_SIZE; 
    this->sender.resize(stringSize);

    stringSize = deserialize_string(received_data, arrayPos, this->receiver, RECEIVER_SIZE);
    arrayPos += RECEIVER_SIZE;
    this->receiver.resize(stringSize);

    stringSize = deserialize_string(received_data, arrayPos, this->subject, SUBJECT_SIZE);
    arrayPos += SUBJECT_SIZE;
    this->subject.resize(stringSize);

    stringSize = deserialize_string(received_data, arrayPos, this->message, messageSize);
    this->message.resize(stringSize);
    
}

Send_prot::~Send_prot() {
}

//returns size needed for buffer to contain all data.
int Send_prot::get_buffersize() {
  return (PROTOCOL_HEADER_SIZE + SENDER_SIZE + RECEIVER_SIZE + SUBJECT_SIZE + this->message.size());
}

//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
void Send_prot::serialize(char* serialized_data) {
  int length = this->get_buffersize();
  int arrayPos = 0;

  //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
  serialized_data[0] = length & 0xFF;
  serialized_data[1] = (length >> 8) & 0xFF;

    //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
    serialized_data[2] = '0'; //setting Protocol type to send_prot.

    arrayPos += PROTOCOL_HEADER_SIZE;

    
    //Write Sender into package.
    serialize_string(serialized_data, arrayPos, this->sender, SENDER_SIZE);
    arrayPos += SENDER_SIZE;

    //Write Receiver into package
    serialize_string(serialized_data, arrayPos, this->receiver, RECEIVER_SIZE);
    arrayPos += RECEIVER_SIZE;

    //Write Subject into package
    serialize_string(serialized_data, arrayPos, this->subject, SUBJECT_SIZE);
    arrayPos += SUBJECT_SIZE;

    //Write Subject into package
    serialize_string(serialized_data, arrayPos, this->message, this->message.size());

    

    /* RECONVERTION TO SHORT FOR LATER
    short test = serialized_data[0] << 8 | serialized_data[1];
    test = ntohs(test);
    */
}

std::string Send_prot::return_sender() {
    return this->sender;
}

std::string Send_prot::return_receiver()
{
    return this->receiver;
}

std::string Send_prot::return_subject()
{
    return this->subject;
}

std::string Send_prot::return_message()
{
    return this->message;
}



//function to read Message typed by user into string.
void Send_prot::typeMessage(std::string &message) {
    std::string buffer = "";

    cout << endl
         << "MESSAGE TYPER" << endl
         << "To end message typing, enter an line containing a single dot." << endl
         << "Essentialy, hit the following keys in succesion: 'Enter' '.' 'Enter'" << endl << endl
         << "Enter message:" << endl;

    while(buffer != ".")
    {
        buffer = "";
        getline(std::cin, buffer);
        message.append(buffer);
        message.push_back('\n');
    }

    for(int i = 0; i < 2; i++) //remove message overhead, produced by end definition of entering a message.
    {
        message.pop_back();
    }
    if(message.empty())
    {
        message = "***EMPTY MESSAGE***";
    }
}


/************************LIST PROTOCOL************************/
List_prot::List_prot(std::string ldapUsername)
{
    this->username = ldapUsername;
}

//Build Object with given serialized data. Probably only used by Server, receiving the data from client.
List_prot::List_prot(char* received_data) {
    
        int arrayPos = 0;
        int stringSize;
    
        //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
        short length = received_data[0] << 8 | received_data[1];
        length = ntohs(length);
    
        arrayPos += PROTOCOL_HEADER_SIZE;
    
        this->username.resize(USERNAME_SIZE);
        
    
        stringSize = deserialize_string(received_data, arrayPos, this->username, USERNAME_SIZE);
        this->username.resize(stringSize);
        
}
    
List_prot::~List_prot() {
}

//returns size needed for buffer to contain all data.
int List_prot::get_buffersize() {
    return (PROTOCOL_HEADER_SIZE + USERNAME_SIZE);
  }


//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
void List_prot::serialize(char* serialized_data) {
    int length = this->get_buffersize();
    int arrayPos = 0;
  
    //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
    serialized_data[0] = length & 0xFF;
    serialized_data[1] = (length >> 8) & 0xFF;
  
      //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
      serialized_data[2] = '1'; //setting Protocol type to List_prot.
  
      arrayPos += PROTOCOL_HEADER_SIZE;
  
      
      //Write Sender into package.
      serialize_string(serialized_data, arrayPos, this->username, USERNAME_SIZE);
  
      /* RECONVERTION TO SHORT FOR LATER
      short test = serialized_data[0] << 8 | serialized_data[1];
      test = ntohs(test);
      */
  }
/************************READ PROTOCOL************************/
Read_prot::Read_prot(std::string ldapUsername) {
    std::string input;
    bool isNumber;

    //fill in strings of the object
    this->username = ldapUsername;
    //get message_nr from user
    do
    {
        isNumber = true;
        cout << "Message Number: ";
        cin  >> input;
    
        cin.clear();
        cin.ignore();

        for (auto i = input.begin(); i != input.end(); i++)
        {
            if(!std::isdigit(*i))
            {
                isNumber = false;
                cout << "ERR: NOT A NUMBER!" << endl << endl;
                break;
            }
        }
    }
    while(!isNumber);
    this->message_nr = input;
}

//Build Object with given serialized data. Probably only used by Server, receiving the data from client.
Read_prot::Read_prot(char* received_data) {
    
    int arrayPos = 0;
    int message_nrSize = 0;
    int stringSize;

    //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
    short length = received_data[0] << 8 | received_data[1];
    length = ntohs(length);
    message_nrSize = (length - (PROTOCOL_HEADER_SIZE + USERNAME_SIZE)); //Subtract fixed constants to determine message_nr length.

    arrayPos += PROTOCOL_HEADER_SIZE;

    this->username.resize(USERNAME_SIZE);
    this->message_nr.resize(length -(PROTOCOL_HEADER_SIZE + USERNAME_SIZE));
    
    stringSize = deserialize_string(received_data, arrayPos, this->username, USERNAME_SIZE);
    this->username.resize(stringSize);

    stringSize = deserialize_string(received_data, arrayPos, this->message_nr, message_nrSize);   
    this->message_nr.resize(stringSize);
}
    
Read_prot::~Read_prot() {
}

//returns size needed for buffer to contain all data.
int Read_prot::get_buffersize() {
    return (PROTOCOL_HEADER_SIZE + USERNAME_SIZE + this->message_nr.size());
  }


//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
void Read_prot::serialize(char* serialized_data) {
    int length = this->get_buffersize();
    int arrayPos = 0;
  
    //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
    serialized_data[0] = length & 0xFF;
    serialized_data[1] = (length >> 8) & 0xFF;
  
      //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
      serialized_data[2] = '2'; //setting Protocol type to Read_prot.
  
      arrayPos += PROTOCOL_HEADER_SIZE;
  
      
      //Write Sender into package.
      serialize_string(serialized_data, arrayPos, this->username, USERNAME_SIZE);
      arrayPos += USERNAME_SIZE;

      serialize_string(serialized_data, arrayPos, this->message_nr, this->message_nr.size());

      /* RECONVERTION TO SHORT FOR LATER
      short test = serialized_data[0] << 8 | serialized_data[1];
      test = ntohs(test);
      */
  }

    std::string Read_prot::return_message_nr()
    {
        return this->message_nr;
    }

    std::string Read_prot::return_username()
    {
        return this->username;
    }

/************************DELETE PROTOCOL************************/
Delete_prot::Delete_prot(std::string ldapUsername) {
    std::string input;
    bool isNumber;

    this->username = ldapUsername;
    //get message_nr from user
    do
    {
        isNumber = true;
        cout << "Message Number: ";
        getline(std::cin, input);

        for (auto i = input.begin(); i != input.end(); i++)
        {
            if(!std::isdigit(*i))
            {
                isNumber = false;
                cout << "ERR: NOT A NUMBER!" << endl << endl;
                break;
            }
        }
    }
    while(!isNumber);
    this->message_nr = input;
}

//Build Object with given serialized data. Probably only used by Server, receiving the data from client.
Delete_prot::Delete_prot(char* received_data) {
    
    int arrayPos = 0;
    int message_nrSize = 0;
    int stringSize;

    //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
    short length = received_data[0] << 8 | received_data[1];
    length = ntohs(length);
    message_nrSize = (length - (PROTOCOL_HEADER_SIZE + USERNAME_SIZE)); //Subtract fixed constants to determine message_nr length.

    arrayPos += PROTOCOL_HEADER_SIZE;

    this->username.resize(USERNAME_SIZE);
    this->message_nr.resize(length -(PROTOCOL_HEADER_SIZE + USERNAME_SIZE));
    
    stringSize = deserialize_string(received_data, arrayPos, this->username, USERNAME_SIZE);
    this->username.resize(stringSize);

    arrayPos += USERNAME_SIZE;

    stringSize = deserialize_string(received_data, arrayPos, this->message_nr, message_nrSize);   
    this->message_nr.resize(stringSize);
}
    
Delete_prot::~Delete_prot() {
}

//returns size needed for buffer to contain all data.
int Delete_prot::get_buffersize() {
    return (PROTOCOL_HEADER_SIZE + USERNAME_SIZE + this->message_nr.size());
  }


//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
void Delete_prot::serialize(char* serialized_data) {
    int length = this->get_buffersize();
    int arrayPos = 0;
  
    //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
    serialized_data[0] = length & 0xFF;
    serialized_data[1] = (length >> 8) & 0xFF;
  
      //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
      serialized_data[2] = '3'; //setting Protocol type to Delete_prot.
  
      arrayPos += PROTOCOL_HEADER_SIZE;
  
      
      //Write Sender into package.
      serialize_string(serialized_data, arrayPos, this->username, USERNAME_SIZE);
      arrayPos += USERNAME_SIZE;

      serialize_string(serialized_data, arrayPos, this->message_nr, this->message_nr.size());

      /* RECONVERTION TO SHORT FOR LATER
      short test = serialized_data[0] << 8 | serialized_data[1];
      test = ntohs(test);
      */
  }

std::string Delete_prot::return_msg_nr()
{
    return this->message_nr;
}

std::string Delete_prot::return_usr()
{
    return this->username;
}

  /************************LOGIN PROTOCOL************************/
  Login_prot::Login_prot(std::string &ldapUsername)
  {
      //fill in strings of the object
      set_with_limit("Username", this->username, USERNAME_SIZE);
      set_with_limit("Password", this->password, PASSWORD_SIZE);
      ldapUsername = this->username;
  }

  //Build Object with given serialized data. Probably only used by Server, receiving the data from client.
  Login_prot::Login_prot(char *received_data)
  {
      int arrayPos = 0;
      int stringSize;

      //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
      short length = received_data[0] << 8 | received_data[1];
      length = ntohs(length);

      arrayPos += PROTOCOL_HEADER_SIZE;

      this->username.resize(USERNAME_SIZE);
      this->password.resize(PASSWORD_SIZE);

      stringSize = deserialize_string(received_data, arrayPos, this->username, USERNAME_SIZE);
      this->username.resize(stringSize);
      arrayPos += USERNAME_SIZE;

      stringSize = deserialize_string(received_data, arrayPos, this->password, PASSWORD_SIZE);
      this->password.resize(stringSize);
  }

  Login_prot::~Login_prot()
  {
  }

  //returns size needed for buffer to contain all data.
  int Login_prot::get_buffersize()
  {
      return (PROTOCOL_HEADER_SIZE + USERNAME_SIZE + PASSWORD_SIZE);
  }

  //given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
  void Login_prot::serialize(char *serialized_data)
  {
      int length = this->get_buffersize();
      int arrayPos = 0;

      //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
      serialized_data[0] = length & 0xFF;
      serialized_data[1] = (length >> 8) & 0xFF;

      //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
      serialized_data[2] = '4'; //setting Protocol type to Login_prot.

      arrayPos += PROTOCOL_HEADER_SIZE;

      //Write Sender into package.
      serialize_string(serialized_data, arrayPos, this->username, USERNAME_SIZE);
      arrayPos += USERNAME_SIZE;

      serialize_string(serialized_data, arrayPos, this->password, PASSWORD_SIZE);

      /* RECONVERTION TO SHORT FOR LATER
      short test = serialized_data[0] << 8 | serialized_data[1];
      test = ntohs(test);
      */
  }

  std::string Login_prot::get_username ()
  {
    return this->username;
  }

  std::string Login_prot::get_password ()
  {
    return this->password;
  }

