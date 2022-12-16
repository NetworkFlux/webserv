#include "../../include/Socket/ServerSocket.hpp"

/*	The socket is being created by the parent class but this contructor
	will setup the sockaddr_in _address to be ready for connection.	*/
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

/*	This destructor will only close the connection file descriptor if it was
	previously opened.	*/
ServerSocket::~ServerSocket()
{
	if (_connection != -1)
		close(_connection);
}

/*	This function will bind the socket with the setup address and make the socket passive
	and responsible to listen for incoming connections.	*/
void	ServerSocket::listeningMode(int maxIncoming)
{
	testConnection(bind(_sockFD, (struct sockaddr *)&_address, sizeof(_address)), "Failed to bind to port.\n");
	testConnection(listen(_sockFD, maxIncoming), "Failed to listen on socket.\n");
}

/*	If the socket is in listening mode it will wait for incoming connections and and read
	from the connection if a connection is established.	*/
void	ServerSocket::grabConnection(void)
{
		_addressLen = sizeof(_address);
		_connection = accept(_sockFD, (struct sockaddr *)&_address, (socklen_t *)&_addressLen);
		testConnection(_connection, "Failed to grab a connection.\n");
}

/*	If a connection has been made this function will attempt to read from the socket
	and output the read content.	*/
std::string	ServerSocket::readConnection(void)
{
	int		bytesRead;

	_request.clear();
	bzero(_buffer, sizeof(_buffer));
	while ((bytesRead = read(_connection, _buffer, BUFFER_SIZE)) > 1)
	{
		_request.append(_buffer);
		if (_buffer[bytesRead - 1] == '\n')
			break;
		bzero(_buffer, sizeof(_buffer));
	}
	if (bytesRead < 0)
	{
		perror("Failed to read,\n");
		exit(EXIT_FAILURE);
	}
	return (_request);
}

/*	This function will send a message to the connection.	*/
void	ServerSocket::giveResponse(std::string message)
{
	send(_connection, message.c_str(), message.size(), 0);
}