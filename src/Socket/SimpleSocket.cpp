# include "../../include/Socket/SimpleSocket.hpp"

/*	This constructor will create the socket with the desired parameters
	and verifies if the creation was succesfull.	*/
SimpleSocket::SimpleSocket(int domain, int type, int protocol)
{
	_sockFD = socket(domain, type, protocol);
	testConnection(_sockFD, "Failed to create the socket.\n");
}

/*	The desctructor will only close the the socket file descriptor.
	It is not responsible for closing whatever was opened by child class.	*/
SimpleSocket::~SimpleSocket()
{
	close(_sockFD);
}

/*	This verification function verifies the output given by system fucntion (read(), write(), socket(), ...).
	It also outputs a given error messages*/
void SimpleSocket::testConnection(int toTest, std::string errorMessage)
{
	if (toTest < 0)
	{
		perror(errorMessage.c_str());
		exit(EXIT_FAILURE);
	}
}