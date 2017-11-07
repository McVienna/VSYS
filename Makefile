all: Server Client
	
Server:
	g++ -Wall -DLDAP_DEPRECATED -std=c++17 -o startServer myserver.cpp s_filehandler.cpp protocols.cpp socketutility.cpp ldaphandler.cpp -lstdc++fs -lldap -llber

Client:
	g++ -Wall -std=c++17 -o startClient myclient.cpp protocols.cpp socketutility.cpp userHandlerIO.cpp -lstdc++fs
	
clean:
	rm -f *.o
