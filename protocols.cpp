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

 Also saves length of package recieved in &transmission_length
*/
int get_protocol(char* recieved_data, unsigned short &transmission_length){
  //check if recieved_data contains valid values.
  if( recieved_data[0] == '\0'
   || recieved_data[1] == '\0'
   || recieved_data[2] == '\0')
    {
      return -1;
    }

  //set transmission length
  transmission_length = recieved_data[0] << 8 | recieved_data[1];
  transmission_length = ntohs(transmission_length);

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
  while(toSet.size() < 0 && toSet.size() > limit);
}

//function to read Message typed by user into string.
void typeMessage(std::string &message) {
  std::string buffer = "";

  cout << "MESSAGE TYPER" << endl
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
  message.erase((message.back()-2), message.back());
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
  this->serialized_data = (char*) malloc(strlen(received_data)*sizeof(char));

  this->serialized_data = received_data;

  for(int i = 2; i < sizeof(received_data); i++)
  {
    std::cout << received_data[i];
  }
  /*for(int i = 2; i < strlen(received_data); i++)
  {
    std::string buffer = "";
    int str = 0;

    if(i < 10)
    {
      this->sender.push_back(received_data[i]);
      std::cout << received_data[i] << std::endl;

      if(received_data[i] == '\0')
      {
        i = 10;
        continue;
      }
    }*/
  }

Send_prot::~Send_prot() {

	  free(this->serialized_data);
}


//returns size needed for buffer to contain all data.
int Send_prot::get_buffersize(){
  return (2+9+9+81+this->message.size()+1);
}

//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
char *Send_prot::serialize() {
    int length = 2 + 9 + 9 + 81 + this->message.size() + 1; //length+sender+reciever+subject+messege
    int arrayPos = 0;

  //allocate correct size for protocol package
  char* tmp_a = (char*) realloc(this->serialized_data, length*sizeof(char));
  if ( tmp_a == NULL ) // realloc has failed
    {
      // error handling
      printf("The re-allocation of array serialized_data has failed. Probably not your fault :(");
      free(this->serialized_data);
      exit(-2);
    }
  else //realloc was successful
    {
     this->serialized_data = tmp_a;
    }
  
    memset(this->serialized_data, '\0', sizeof(&this->serialized_data));

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

/* LIST PROTOCOL */
/* READ PROTOCOL */
/* DELETE PROTOCOL */
