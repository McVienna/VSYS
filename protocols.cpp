#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "protocols.h"


using namespace std;

/************************UTILITY FUNCTIONS************************/

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

//sets a string with limit being max amount of letters.
void set_with_limit(std::string name, std::string &toSet, unsigned int limit) {
    int loop = 0;

    do
    {
        if(loop > 0)
        {
            cout << "Invalid length of argument! Watch the limit in parantheses!" << endl << endl;
        }
        cout << name << "(" << limit << "): ";
        cin  >> toSet;

        loop++;
    }
    while(toSet.size() < 1 || toSet.size() > limit);
}

//Given a string, enter each digit into a char Array serializing it. "setPosition" gives starting Index within the Array.
void serialize_string(char* serialized_Array, int setPosition, std::string toSerialize, int maxStringLength) {

    for(unsigned int i = 0; i < (unsigned int) maxStringLength; i++)
    {
        if(toSerialize.size() > i) serialized_Array[setPosition+i] = toSerialize[i];
        else                  break;
    }
}

//Given an Array, and a starting Indet('setPosition'), derseialize into String, fpr max of string Length
void deserialize_string(char* serialized_Array, int setPosition, std::string &intoString, int maxStringLength) {

    for(unsigned int i = 0; i < (unsigned int) maxStringLength; i++)
    {
        if(serialized_Array[setPosition+i] != 0) intoString[i] = serialized_Array[setPosition+i];
        else                  break;
    }
}



//function to read Message typed by user into string.
void typeMessage(std::string &message) {
    std::string buffer = "";

    cout << endl
         << "MESSAGE TYPER" << endl
         << "To end message typing, enter an line containing a single dot." << endl
         << "Essentialy, hit the following keys in succesion: 'Enter' '.' 'Enter'" << endl << endl
         << "Enter message:" << endl;

    while(buffer != ".")
    {
        buffer = "";
        cin >> buffer;
        message.append(buffer);
        message.push_back('\n');
    }

    for(int i = 0; i < 3; i++) //remove message overhead, produced by end definition of entering a message.
    {
        message.pop_back();
    }
}







/************************PROTOCOLS************************/


/* SEND PROTOCOL */
Send_prot::Send_prot() {
    //fill in strings of the object
    set_with_limit("Sender", this->sender, SENDER_SIZE);
    set_with_limit("Receiver", this->receiver, RECEIVER_SIZE);
    set_with_limit("Subject", this->subject, SUBJECT_SIZE);
    typeMessage(this->message);
}

//Build Object with given serialized data. Probably only used by Server, receiving the data from client.
Send_prot::Send_prot(char* received_data) {

    int arrayPos = 0;
    int messageSize = 0;

    //Short is 2 Bytes in size, so needs to be parsed out of 2 char values. Gets stored in HEX by Sender.
    short length = received_data[0] << 8 | received_data[1];
    length = ntohs(length);
    messageSize = (length - (PROTOCOL_HEADER_SIZE + SENDER_SIZE + RECEIVER_SIZE + SUBJECT_SIZE)); //Subtract fixed constants to determine message length.

    arrayPos += PROTOCOL_HEADER_SIZE;

    /*
    std::string _temp;

    this->serialized_data = new char[length];

    strcpy(this->serialized_data, received_data);

    for(int i = 3; i < length; i++)
      {
        _temp.resize (length);
        if(received_data[i] != '\0')
        {

            _temp[i-3] = received_data[i];
        }
          else
        {
            _temp[i-3] = '*';
        }
      }
      */

    this->sender.resize(8);
    this->receiver.resize(8);
    this->subject.resize(80);
    this->message.resize(messageSize);

    deserialize_string(received_data, arrayPos, this->sender, SENDER_SIZE);
    arrayPos += SENDER_SIZE;
    cout << "SENDER: " << this->sender << endl;

    deserialize_string(received_data, arrayPos, this->receiver, RECEIVER_SIZE);
    arrayPos += RECEIVER_SIZE;
    cout << "RECEIVER: " << this->receiver << endl;

    deserialize_string(received_data, arrayPos, this->subject, SUBJECT_SIZE);
    arrayPos += SUBJECT_SIZE;
    cout << "SUBJECT: " << this->subject << endl;

    deserialize_string(received_data, arrayPos, this->message, messageSize);
    cout << "Message: " << this->message << endl;
    
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

/* LIST PROTOCOL */
/* READ PROTOCOL */
/* DELETE PROTOCOL */
