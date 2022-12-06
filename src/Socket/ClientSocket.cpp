#include "../../include/Socket/ClientSocket.hpp"

ClientSocket::ClientSocket(int domain, int type, int protocol, int port)
	: SimpleSocket(domain, type, protocol)
{
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
	_address.sin_port = htons(port);
}

ClientSocket::~ClientSocket() {}

void	ClientSocket::setTargetIP(char *targetIP)
{
	if (inet_pton(_address.sin_family, targetIP, &_address.sin_addr) <= 0)
		testConnection(-1, "Failed to translate the IP Address.\n");
}

void	ClientSocket::connectToServer(void)
{
	testConnection(connect(_sockFD, (struct sockaddr *)&_address, sizeof(_address)), "Failed to connecto to server.\n");
}

void	ClientSocket::sendMessageToServer(std::string message)
{
	int	n;

	n = write(_sockFD, message.c_str(), message.size());
	testConnection(n != (int)message.size(), "Failed to send the message.\n");
}

void	ClientSocket::getServerResponse(void)
{
	int	n;

	bzero(_serverResponse, sizeof(_serverResponse));
	while ((n = read(_sockFD, _serverResponse, BUFFER_SIZE)) > 0)
		std::cout << _serverResponse;
	testConnection(n, "Failed to read the server's response.\n");
}