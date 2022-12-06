#pragma once

#include "SimpleSocket.hpp"

#define BUFFER_SIZE 4096

/*	This Socket must be utilized by a client.	*/
class ClientSocket : public SimpleSocket
{
	// Member Variables
	private:
		sockaddr_in	_address;
		char		_serverResponse[BUFFER_SIZE];

	// Constructors
	public:
		ClientSocket(int domain, int type, int protocol, int port);
		~ClientSocket();

	// Member Functions
	public:
		void	setTargetIP(char *targetIP);
		void	connectToServer(void);
		void	sendMessageToServer(std::string message);
		void	getServerResponse(void);
};