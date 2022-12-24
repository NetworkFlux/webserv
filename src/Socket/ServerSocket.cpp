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
	std::cout << "closing";
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
	int new_socket = 0;
	while (new_socket != -1)
	{
		_addressLen = sizeof(_address);
		_connection = accept(_sockFD, (struct sockaddr *)&_address, (socklen_t *)&_addressLen);
		new_socket = _connection;
		if (new_socket < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("accept failed");
				exit(EXIT_FAILURE);
			}
			new_socket = -1;
		}
		else
			_socket_clients.push_back(new_socket);
	}
}

/*	If a connection has been made this function will attempt to read from the socket
	and output the read content.	*/
std::string		ServerSocket::readConnection(struct pollfd	*ptr_tab_poll)
{
	int		bytesRead;

	_request.clear();
	bzero(_buffer, sizeof(_buffer));
	while ((bytesRead = read(ptr_tab_poll->fd, _buffer, BUFFER_SIZE)) > 1)
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
int		ServerSocket::giveResponse(struct pollfd *ptr_tab_poll, std::string message)
{
	int ret;
	int	fd = ptr_tab_poll->fd;

	ret = send(fd, message.c_str(), message.size(), 0);
	ptr_tab_poll->events = POLLIN;
	return ret;
}

void	ServerSocket::socketConf()
{
	int on = 1;
	int rc;

	rc = setsockopt(_sockFD, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  	if (rc < 0)
  	{
		perror("setsockopt() failed");
		close(_sockFD);
		exit(-1);
  	}
	rc = fcntl(_sockFD, F_SETFL, O_NONBLOCK);// Set socket to be nonblocking
	if (rc < 0)
  	{
		perror("fcntl() failed");
		close(_sockFD);
		exit(-1);
  	}
}

void	ServerSocket::shrink_socket_clients(int to_find)
{
	for (std::vector<int>::iterator it = _socket_clients.begin(); it !=  _socket_clients.end() ; it++)
	{
		if (*it == to_find)
		{
			_socket_clients.erase(it);
			return ;
		}
	}
}


std::vector<int>&	ServerSocket::get_socket_client()
{
	return _socket_clients;
}