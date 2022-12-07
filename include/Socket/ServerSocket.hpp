#pragma once

#include "SimpleSocket.hpp"

#define BUFFER_SIZE 4096

/*	This Socket must be utilized by a server	*/
class ServerSocket : public SimpleSocket
{
	// Member Variables
	private:
		sockaddr_in	_address;
		int			_addressLen;
		int			_connection; // File Descriptor to the established connection
		char		_buffer[BUFFER_SIZE];

	// Unused Constructors
	private:
		ServerSocket();
		ServerSocket(const ServerSocket& src);

	// Constructors
	public:
		ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface);
		~ServerSocket();

	// Unused Operator Overload
		ServerSocket&	operator=(const ServerSocket& other);

	// Member Functions
	public:
		void	listeningMode(int maxIncoming);
		void	grabConnection(void);
		void	readConnection(void);
		void	giveResponse(std::string message);
};