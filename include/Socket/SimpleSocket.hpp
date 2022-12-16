#pragma once

#include <iostream>
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>


/*	This is the Base Socket Class, it handels the creation and deltion
	of not yet connected sockets	*/
class SimpleSocket
{
	// Member Variables
	protected:
		int	_sockFD; // File Descriptor to the Socket

	// Unused Constructors
	private:
		SimpleSocket();
		SimpleSocket(const SimpleSocket& src);

	// Constructors
	public:
		SimpleSocket(int domain, int type, int protocol);
		~SimpleSocket();

	// Unused Operator Overload
	private:
		SimpleSocket&	operator=(const SimpleSocket& other);

	// Helper Functions
	protected:
		void	testConnection(int toTest, std::string errorMessage);
};