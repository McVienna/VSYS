#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
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
int get_protocol(char* recieved_data){
    //check if recieved_data contains valid values.
    if(recieved_data[2] == '\0')
    {
        return -1;
    }

    return ((int) (recieved_data[2] - '0'));
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
    cout << message << endl;
    for(int i = 0; i < 3; i++)
      {
        message.pop_back();
      }
    cout << message << endl;
}







/************************PROTOCOLS************************/


/* SEND PROTOCOL */
Send_prot::Send_prot() {

    //alloc small space for message, will bereallocated later when constructing serialized data.
    this->serialized_data = (char*) malloc(10*sizeof(char));
    if (serialized_data == NULL)
    {
        // error handling
        printf("Allocation of memory has failed. Probably not your fault :(");
        exit(-1);
    }

    //fill in strings of the object
    set_with_limit("Sender", this->sender, 8);
    set_with_limit("Reciever", this->reciever, 8);
    set_with_limit("Subject", this->subject, 80);
    typeMessage(this->message);
}

Send_prot::Send_prot(char* received_data) {

    short length = received_data[0] << 8 | received_data[1];

    length = ntohs(length);

    std::string _temp;

    this->serialized_data = new char[length];

    strcpy(this->serialized_data, received_data);

    
    for(int i = 3; i < length; i++)
      {
        _temp.resize (length);
        _temp[i-3] = received_data[i];
      }


   this->sender = _temp.substr(0, 8);
      cout << "Sender:" << this->sender << endl;
   this->reciever = _temp.substr(7, 8);
      cout << "Reciever:" << this->reciever << endl;
  this->subject = _temp.substr(15, 80);
      cout << "Subject:" << this->subject << endl;
    this->message = _temp.substr(99, length);
      cout << "Message:" << endl << this->message << endl;

    
    
    
    

}

Send_prot::~Send_prot() {

    free(this->serialized_data);
}


//returns size needed for buffer to contain all data.
int Send_prot::get_buffersize(){
  return (2+1+8+8+80+this->message.size());
}

//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
char *Send_prot::serialize() {
  int length = 2 + 1 + 8 + 8 + 80 + this->message.size(); //length+sender+reciever+subject+messege
  int arrayPos = 0;

  //allocate correct size for protocol package
  char* tmp_a = this->serialized_data;
  serialized_data = new char[length];
  delete tmp_a;

  memset(this->serialized_data, '\0', length);


  //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
  serialized_data[0] = length & 0xFF;
  serialized_data[1] = (length >> 8) & 0xFF;

    //set protocol type. See protocols.h function 'get_protocol(char* serialized_data)' for further reference.
    serialized_data[2] = '0'; //setting Protocol type to send_prot.

    //Write Sender into package.
    arrayPos = 3;
    for(unsigned int i = 0; i < 8; i++)
    {
        if(sender.size() > i) serialized_data[arrayPos] = sender[i];
        else                  break;
        arrayPos++;
    }
    //Write Reciever into package
    arrayPos = 11;
    for(unsigned int i = 0; i < 8; i++)
    {
        if(reciever.size() > i) serialized_data[arrayPos] = reciever[i];
        else                    break;
        arrayPos++;
    }
    //Write Subject into package
    arrayPos = 19;
    for(unsigned int i = 0; i < 80; i++)
    {
        if(subject.size() > i) serialized_data[arrayPos] = subject[i];
        else                   break;
        arrayPos++;
    }
    //Write Subject into package
    arrayPos = 99;
    for(unsigned int i = 0; i < this->message.size(); i++)
    {
        if(message.size() > i) serialized_data[arrayPos] = message[i];
        else                   break;
        arrayPos++;
    }
    /* RECONVERTION TO SHORT FOR LATER
    short test = serialized_data[0] << 8 | serialized_data[1];
    test = ntohs(test);
    */
    return serialized_data;
}

std::string Send_prot::return_sender()
{
    return this->sender;
}

/* LIST PROTOCOL */
/* READ PROTOCOL */
/* DELETE PROTOCOL */