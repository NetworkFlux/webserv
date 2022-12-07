#include "../../include/Socket/ClientSocket.hpp"

/*	The socket is being created by the parent class but this contructor
	will setup the sockaddr_in _address to be ready for connection.	*/
ClientSocket::ClientSocket(int domain, int type, int protocol, int port)
	: SimpleSocket(domain, type, protocol)
{
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
	_address.sin_port = htons(port);
}

ClientSocket::~ClientSocket() {}

/*	This functions will set the desired IP to be linked with the socket.	*/
void	ClientSocket::setTargetIP(char *targetIP)
{
	if (inet_pton(_address.sin_family, targetIP, &_address.sin_addr) <= 0)
		testConnection(-1, "Failed to translate the IP Address.\n");
}

/*	This function will establish the connection to the server pointed by setTargetIP().	*/
void	ClientSocket::connectToServer(void)
{
	testConnection(connect(_sockFD, (struct sockaddr *)&_address, sizeof(_address)), "Failed to connecto to server.\n");
}

/*	This function will send the desired message or request to the server.	*/
void	ClientSocket::sendMessageToServer(std::string message)
{
	int n = write(_sockFD, message.c_str(), message.size());
	testConnection(n != (int)message.size(), "Failed to send the message.\n");
}

/*	This function reads from the server and outputs the read content.	*/
void	ClientSocket::getServerResponse(void)
{
	int	n;

	bzero(_serverResponse, sizeof(_serverResponse));
	while ((n = read(_sockFD, _serverResponse, BUFFER_SIZE)) > 0)
		std::cout << _serverResponse;
	testConnection(n, "Failed to read the server's response.\n");
}