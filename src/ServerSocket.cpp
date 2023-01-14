#include "../include/WebServer.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface, size_t serv_index)
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
	_serv_index = serv_index;
}

ServerSocket::~ServerSocket()
{
	std::cout << "Closing Server";
	if (_connection != -1)
		close(_connection);
}

size_t		ServerSocket::get_serv_index()
{
	return (_serv_index);
}

void	ServerSocket::listeningMode(int maxIncoming)
{
	if ((bind(_sockFD, (struct sockaddr *)&_address, sizeof(_address))) < 0)
		throw std::runtime_error("Failed to bind to port");
	if ((listen(_sockFD, maxIncoming)) < 0)
		throw std::runtime_error("Failed to listen on socket");
}

int	ServerSocket::grabConnection(void)
{
	int new_socket = 0;
	struct sockaddr_in cli_addr_tmp;
	socklen_t clilen_tmp = sizeof(cli_addr_tmp);


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

	getsockname(_connection, (struct sockaddr *) &cli_addr_tmp, &clilen_tmp);
	int client_port = ntohs(cli_addr_tmp.sin_port);

	std::cout << "Connection accepted from " << inet_ntoa(_address.sin_addr) << ":" << client_port << std::endl;
	return (htons(_address.sin_port));
}

/*	If a connection has been made this function will attempt to read from the socket
	and output the read content.	*/
int ServerSocket::readConnection(int fd, std::string *req)
{
    int bytesRead;
    _request.clear();
    bzero(_buffer, sizeof(_buffer));
    while (true)
    {
        bytesRead = read(fd, _buffer, BUFFER_SIZE);
        if (bytesRead > 0)
        {
            _request.append(_buffer);
            if (_buffer[bytesRead - 1] == '\n')
                break;
			std::string transferEncodingStr = "Transfer-Encoding: chunked";
            std::size_t transferEncodingPos = _request.find(transferEncodingStr);
            if (transferEncodingPos != std::string::npos) {
                if (_request.substr(_request.length()-5) == "0\r\n\r\n") {
                    break;
                }
            }
			std::string contentLengthStr = "Content-Length: ";
            std::size_t contentLengthPos = _request.find(contentLengthStr);
            if (contentLengthPos != std::string::npos) {
                size_t contentLength = stoi(_request.substr(contentLengthPos + contentLengthStr.length()));
                if (_request.length() - _request.rfind("\r\n\r\n") - 4 >= contentLength) {
                    break;
                }
            }

			bzero(_buffer, sizeof(_buffer));
        }
        else if (bytesRead == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue; // read would block, try again
            else
            {
                // other error occurred
                perror("Failed to read");
                exit(EXIT_FAILURE);
            }
        }
        else
		{
            break; // read returned 0, connection closed
		}
	}
	*req = _request;
    return (bytesRead);
}

// int ServerSocket::readConnection(int fd, std::string *req)
// {
//     int bytesRead;
//     _request.clear();
//     bzero(_buffer, sizeof(_buffer));
//     while (true) {
//         bytesRead = read(fd, _buffer, BUFFER_SIZE);
// 		std::cout << "Read : " << _buffer << "end" << std::endl;
//         if (bytesRead > 0) {
//             _request.append(_buffer);

// 			if (_request.find("\r\n") != std::string::npos) {
//                 break;
//             }
//             std::string transferEncodingStr = "Transfer-Encoding: chunked";
//             std::size_t transferEncodingPos = _request.find(transferEncodingStr);
//             if (transferEncodingPos != std::string::npos) {
//                 // check if the last chunk has been received
//                 if (_request.substr(_request.length()-5) == "0\r\n\r\n") {
//                     break;
//                 }
//             }
// 			 // Check if we have reached the end of the request body
// 			std::string contentLengthStr = "Content-Length: ";
//             std::size_t contentLengthPos = _request.find(contentLengthStr);
//             if (contentLengthPos != std::string::npos) {
//                 size_t contentLength = stoi(_request.substr(contentLengthPos + contentLengthStr.length()));
//                 if (_request.length() - _request.rfind("\r\n\r\n") - 4 >= contentLength) {
//                     break;
//                 }
//             }

//             bzero(_buffer, sizeof(_buffer));
//         } else if (bytesRead == -1) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                 // read would block, try again
//                 continue;
//             } else {
//                 // other error occurred
//                 perror("Failed to read");
//                 exit(EXIT_FAILURE);
//             }
//         } else {
//             break; // read returned 0, connection closed
//         }
//     }
//     *req = _request;
//     return (bytesRead);
// }


/*	This function will send a message to the connection.	*/
int		ServerSocket::giveResponse(int fd, std::string message)
{
	int ret;

	std::cout << YELLOW_B << "Response: " << std::endl << YELLOW << getHttpRequestWithoutBody(message).c_str() << NONE << std::endl << std::endl;
	ret = send(fd, message.c_str(), message.size(), 0);
	std::cout << "Retour " << ret << std::endl;
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