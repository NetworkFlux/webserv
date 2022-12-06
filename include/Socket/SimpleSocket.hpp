#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*	This is the Base Socket Class, it handels the creation and deltion
	of not yet connected sockets	*/
class SimpleSocket
{
	// Member Variables
	protected:
		int	_sockFD; // File Descriptor to the Socket

	// Constructors
	public:
		SimpleSocket(int domain, int type, int protocol);
		~SimpleSocket();

	// Helper Functions
	protected:
		void	testConnection(int toTest, std::string errorMessage);
};