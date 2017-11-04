#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

//Menu-Formatting Parameters
#define MAX_LINE_WIDTH 86
#define TITLE_LENGTH   9

using namespace std;

//prints Menu 
void printMenu() {
  int titlePadding = ((MAX_LINE_WIDTH / 2) + (TITLE_LENGTH / 2));
	int linePadding  = (MAX_LINE_WIDTH - 2);

	/*
	Each block represents one line. Use this Scheme to insert a Line:
		<< "|" << std::setw(linePadding)
		<< "PUT YOUR TEXT IN HERE"
		<< "|" << std::endl

		Insert a space instead of Text to produce an empty line.
	*/

	std::cout << std::left << std::endl << std::endl

		//TITLE
		<< "+" << std::setfill ('-') << std::right
		<< std::setw(titlePadding - 1)
		<< "TW-MAILER"
		<< std::setw((titlePadding - TITLE_LENGTH))
		<< "+"
		<< std::endl << std::setfill (' ') << std::left

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl

		<< "|" << std::setw(linePadding)
		<< "Possible Inputs:"
		<< "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl

		<< "|" << std::setw(linePadding)
		<< "<1> SEND: "
		<< "|" << std::endl

      << "|" << std::setw(linePadding)
      << "Send a message to a user."
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		
    
    << "|" << std::setw(linePadding)
		<< "<2> LIST: "
		<< "|" << std::endl

      << "|" << std::setw(linePadding)
      << "List all messages in your mail directory."
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		

		<< "|" << std::setw(linePadding)
		<< "<3> READ: "
		<< "|" << std::endl

      << "|" << std::setw(linePadding)
      << "Display content of a given message."
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		

    << "|" << std::setw(linePadding)
    << "<4> DEL: "
    << "|" << std::endl

      << "|" << std::setw(linePadding)
      << "Delete a specific message."
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		     

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		    
    
    << "|" << std::setw(linePadding)
    << "<0> QUIT: "
    << "|" << std::endl

      << "|" << std::setw(linePadding)
      << "Terminate this program."
      << "|" << std::endl

      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl		      
        
    << "|" << std::setw(linePadding)
    << " "
    << "|" << std::endl

    << "|" << std::setw(linePadding)
    << "Note: To specify a request/input, use the index number or the complete command word."
    << "|" << std::endl
    
      << "|" << std::setw(linePadding)
      << " "
      << "|" << std::endl

		<< "+" << std::setfill ('-') << std::setw(linePadding)
		<< "-"
		<< "+"
		<< std::endl << std::endl;
}

//Get Request
int getRequestInput() {
  std::string input;

  while (1)
  {
      input = "";
      cout << "Enter Request: ";
      cin  >> input;
      cout << endl;

      std::cin.clear ();
      std::cin.ignore ();

      std::transform(input.begin(), input.end(), input.begin(), ::toupper);

      if(input == "1" || input == "SEND")
      {
        return 0;
      }
      else if (input == "2" || input == "LIST")
      {
        return 1;
      }
      else if (input == "3" || input == "READ")
      {
        return 2;
      }
      else if (input == "4" || input == "DEL")
      {
        return 3;
      }
      else if (input == "0" || input == "QUIT")
      {
        return -1;
      }
      else
      {
         cout << endl << "ERR: INVALID INPUT!" << endl;
      }
  }

}