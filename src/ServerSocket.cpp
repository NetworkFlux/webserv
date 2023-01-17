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


void ServerSocket::chunk_message( std::string *message)
{
	std::string header_name = "Content-Length";
    size_t header_pos = message->find(header_name);
    if (header_pos != std::string::npos) {
        message->erase(header_pos, message->find("\r\n", header_pos) - header_pos + 2);
    }
    message->insert(message->find("\r\n", message->find("\r\n")) + 2, "Transfer-Encoding: chunked\r\n");
	std::cout << YELLOW_B << "Response: " << std::endl << YELLOW << getHttpRequestWithoutBody(*message).c_str() << NONE << std::endl << std::endl;
}

/*	This function will send a message to the connection.	*/
int ServerSocket::giveResponse(int fd, std::string message)
{
	int ret;
	size_t chunk_size = 8192;
    size_t total_sent = 0;
	size_t sent = 0;
	bool first_chunk = true;

    if (message.size() < chunk_size)
	{
        std::cout << YELLOW_B << "Response: " << std::endl << YELLOW << getHttpRequestWithoutBody(message).c_str() << NONE << std::endl << std::endl;
        total_sent = send(fd, message.c_str(), message.size(), 0);
    }
	else
	{
		chunk_message(&message);
		while (sent < message.length())
		{
			int to_send = std::min((int)chunk_size, (int)(message.length() - sent));
			std::stringstream ss;
			std::string chunk;
			
			if (first_chunk)
			{
				size_t headers_end = message.find("\r\n\r\n");
				ss << std::hex << to_send - (headers_end + 4);
       			std::string chunk_size = ss.str();
				chunk = message.substr(0, headers_end + 4) + chunk_size + "\r\n" + message.substr(headers_end + 4, (to_send - (headers_end + 4))) + "\r\n";
				first_chunk = false;
			}
			else
			{
				ss << std::hex << to_send;
        		std::string chunk_size = ss.str();
				chunk = chunk_size + "\r\n" + message.substr(sent, to_send) + "\r\n";
			}
			size_t bytes_sent = 0;
			do {
				ret = send(fd, chunk.data() + bytes_sent, chunk.length() - bytes_sent, MSG_DONTWAIT);
				if (ret == -1)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
						continue;
				}
				else
					bytes_sent += ret;

			} while ((size_t)bytes_sent < chunk.length());
			total_sent += bytes_sent;
			sent += to_send;
		}
		send(fd, "0\r\n\r\n", 5, 0);
	}
	return total_sent;
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