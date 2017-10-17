#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "protocols.h"

/* SEND PROTOCOL */
Send_prot::Send_prot() {
this->serialized_data = (char*) malloc(10*sizeof(char));
if (serialized_data == NULL)
  {
  // error handling 
  printf("Allocation of memory has failed. Probably not your fault :(");
  exit(-1);
  }
}

Send_prot::Send_prot(char* serialized_data) {

}

Send_prot::~Send_prot() {
  
}

void Send_prot::set_data() {
  
}

//given all members of the Object are set, serialize all Data fot the networktransfer in a char* buffer array;
char* Send_prot::serialize() {
  int length = 2+9+9+81+this->message.size()+1; //length+sender+reciever+subject+messege
  int arrayPos = 0;

  //allocate correct size for protocol package
  char* tmp_a = (char*) realloc(this->serialized_data, 10000*sizeof(char));
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
  
  memset (this->serialized_data, '\0', sizeof(&this->serialized_data));

  //write short, indicating length of package, to first 2 Bytes of serialized_data. Bitwise to newort order automatically
  serialized_data[0] = length & 0xFF;
  serialized_data[1] = (length >> 8) & 0xFF;

  //Write Sender into package.
  arrayPos = 2;
  for(unsigned int i = 0; i < 8; i++)
    {
      if(sender.size() > i) serialized_data[arrayPos] = sender[i];
      else                  break;
      arrayPos++;
    }
  //Write Reciever into package
  arrayPos = 10;
  for(unsigned int i = 0; i < 8; i++)
    {
      if(reciever.size() > i) serialized_data[arrayPos] = reciever[i];
      else                    break;
      arrayPos++;
    }
  //Write Subject into package
  arrayPos = 18;
  for(unsigned int i = 0; i < 80; i++)
    {
      if(subject.size() > i) serialized_data[arrayPos] = subject[i];
      else                   break;
      arrayPos++;
    }
  //Write Subject into package
  arrayPos = 98;
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