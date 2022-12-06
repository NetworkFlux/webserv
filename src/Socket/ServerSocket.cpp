#include "../../include/Socket/ServerSocket.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface)
	: SimpleSocket(domain, type, protocol)
{
	_connection = -1;
	_addressLen = 0;
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
	_address.sin_addr.s_addr = htonl(interface);
	_address.sin_port = htons(port);
}

ServerSocket::~ServerSocket()
{
	if (_connection != -1)
		close(_connection);
}

void	ServerSocket::listening(int maxIncoming)
{
	testConnection(bind(_sockFD, (struct sockaddr *)&_address, sizeof(_address)), "Failed to bind to port.\n");
	testConnection(listen(_sockFD, maxIncoming), "Failed to listen on socket.\n");
}

void	ServerSocket::grabConnection(void)
{
	for ( ; ; )
	{
		_addressLen = sizeof(_address);
		_connection = accept(_sockFD, (struct sockaddr *)&_address, (socklen_t *)&_addressLen);
		testConnection(_connection, "Failed to grab a connection.\n");
		readConnection();
	}
}

void	ServerSocket::readConnection(void)
{
	int		bytesRead;

	bzero(_buffer, sizeof(_buffer));
	while ((bytesRead = read(_connection, _buffer, BUFFER_SIZE)) > 1)
	{
		std::cout << _buffer;
		if (_buffer[bytesRead - 1] == '\n')
			break;
		bzero(_buffer, sizeof(_buffer));
	}
	if (bytesRead < 0)
	{
		perror("Failed to read,\n");
		exit(EXIT_FAILURE);
	}
	giveResponse("HTTP/1.0 200 OK\r\n\r\nHello");
}

void	ServerSocket::giveResponse(std::string message)
{
	send(_connection, message.c_str(), message.size(), 0);
}