#include "../include/WebServer.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface)
{
	_sockFD = socket(domain, type, protocol);
	if (_sockFD == -1)
		throw std::runtime_error("Socket creation failed");
	_connection = -1;
	_addressLen = 0;
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
	_address.sin_addr.s_addr = htonl(interface);
	_address.sin_port = htons(port);
}

ServerSocket::~ServerSocket()
{
	std::cout << "Closing Server";
	if (_connection != -1)
		close(_connection);
}

void	ServerSocket::listeningMode(int maxIncoming)
{
	if ((bind(_sockFD, (struct sockaddr *)&_address, sizeof(_address))) < 0)
		throw std::runtime_error("Failed to bind to port");
	if ((listen(_sockFD, maxIncoming)) < 0)
		throw std::runtime_error("Failed to listen on socket");
}

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
		std::cout << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	return (_request);
}

/*	This function will send a message to the connection.	*/
int		ServerSocket::giveResponse(struct pollfd *ptr_tab_poll, std::string message)
{
	int ret;
	int	fd = ptr_tab_poll->fd;

	std::cout << YELLOW_B << "Response: " << std::endl << YELLOW << message.c_str() << NONE << std::endl << std::endl;
	ret = send(fd, message.c_str(), message.size(), 0);
	ptr_tab_poll->events = POLLIN;
	return ret;
}

void	ServerSocket::socketConf()
{
	int on = 1;
	int rc;

	rc = setsockopt(_sockFD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
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

std::vector<int>&	ServerSocket::get_socket_client()
{
	return _socket_clients;
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

int		ServerSocket::get_sock_fd(void)
{
	return (_sockFD);
}