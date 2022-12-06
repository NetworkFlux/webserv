# include "../../include/Socket/SimpleSocket.hpp"

SimpleSocket::SimpleSocket(int domain, int type, int protocol)
{
	_sockFD = socket(domain, type, protocol);
	testConnection(_sockFD, "Failed to create the socket.\n");
}

SimpleSocket::~SimpleSocket()
{
	close(_sockFD);
}

void SimpleSocket::testConnection(int toTest, std::string errorMessage)
{
	if (toTest < 0)
	{
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
}