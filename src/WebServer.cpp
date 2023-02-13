#include "../include/WebServer.hpp"

WebServer::WebServer(ServerConfig& config) : _sockets_list(), _str_req(), _str_rep()
{
	_config = &config;
	_chunk_size = 8192;
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < _sockets_list.size(); i++)
		delete _sockets_list[i];
}

void	WebServer::printInfo(void)
{
	_config->printInfo();
}

/* Create all server instances */
void	WebServer::createServers(void)
{
	_kq = kqueue();
	if (_kq == -1)
		throw std::runtime_error("Error creating kqueue");

	for (size_t j = 0; j < _config->_servConf.size(); j++)
	{
		int i=0;
		for (std::multimap<std::string, std::string>::iterator it = _config->_servConf[j]._listen.begin(); it != _config->_servConf[j]._listen.end(); it++)
		{
			try {
				ServerSocket* s = new ServerSocket(AF_INET, SOCK_STREAM, 0, stoi(it->second), INADDR_ANY, j);
				s->socketConf();
				s->listeningMode(0);

				struct kevent ev;
				EV_SET(&ev, s->get_sock_fd(), EVFILT_READ, EV_ADD, 0, 0, nullptr);
				_fd_map[s->get_sock_fd()].events = ev;
				if (kevent(_kq, &ev, 1, nullptr, 0, nullptr) == -1)
					throw std::runtime_error("Error adding event to kqueue");
				
				_sockets_list.push_back(s);
			}
			catch(std::runtime_error& e) {
				std::cerr <<"Server " << (j+1) << "." << i << " error : " << e.what() << std::endl;
				break;
			}
			i++;
		}
	}
}

/* Create the poll and loop through the socket to monitor events */
void WebServer::runServers()
{
	while (true)
  	{
		struct kevent evlist[1024];
		// Wait for events on the kqueue
		int n = kevent(_kq, NULL, 0, evlist, 1024, NULL);
		if (n == -1)
			throw std::runtime_error("Error waiting for events");
		// Process the events
		for (int i = 0; i < n; ++i)
		{
			int fd = evlist[i].ident;
			int filter = evlist[i].filter;
			if (filter == EVFILT_READ || filter == EVFILT_WRITE) {
				handleServer(fd, filter);
			}
		}
	}
}


/* This function goes through every socket for each ports and for each
 1/ If a new connection came, grab it and add it to the kqueue
 2/ It it is an existing socket, read or send and change event triggering */
void WebServer::handleServer(int fd, int filter)
{	
	int		ret;

	for (std::vector<ServerSocket*>::iterator it = _sockets_list.begin(); it != _sockets_list.end(); it++)
	{
		ServerSocket *current = *it.base();
		std::vector<int> & current_socket_client = current->get_socket_client();
		int old_size = current_socket_client.size();
	
		if (fd == current->get_sock_fd())
		{
			current->grabConnection();
			for (size_t i = old_size; i < current_socket_client.size(); i++)
			{
				struct kevent event;
        		EV_SET(&event, current_socket_client[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
        		_fd_map[current_socket_client[i]].events = event;
				if (kevent(_kq, &event, 1, NULL, 0, NULL) == -1)
          			std::cerr << "Error adding socket to kqueue: " << std::strerror(errno) << std::endl;
			}
		}
		else if (std::find(current_socket_client.begin(), current_socket_client.end(), fd) != current_socket_client.end())
		{
			if (filter == EVFILT_READ)
			{
				ret = current->readConnection(fd, &_str_req);
				_fd_map[fd].req.append(_str_req);
				_str_req.clear();
				if (ret == 0)
				{
					std::cout << BLUE << "Client (fd : " << fd << ") closed connection" << BLUE_B << std::endl;
					close_connection(fd, current);
					return ;
				}
				if (ret == -1)
				{	
					std::cout << BLUE << "Error reading client (fd : " << fd << "), closing connection" << BLUE_B << std::endl;
					close_connection(fd, current);
					return ;
				}

				std::string transferEncodingStr = "Transfer-Encoding: chunked";
				std::string contentLengthStr = "Content-Length: ";
				if ((_fd_map[fd].req.find("GET") != std::string::npos) || (_fd_map[fd].req.find("DELETE") != std::string::npos) || (_fd_map[fd].req.find("HEAD") != std::string::npos))
				{
					if (_fd_map[fd].req.find("\r\n\r\n") != std::string::npos)
						finished_request(fd, current->get_serv_index());
				}
				else if (_fd_map[fd].req.find(contentLengthStr) != std::string::npos) {
				    size_t contentLength = stoi(_fd_map[fd].req.substr(_fd_map[fd].req.find(contentLengthStr) + contentLengthStr.length()));
				    if (_fd_map[fd].req.length() - _fd_map[fd].req.find("\r\n\r\n") - 4 >= contentLength)
				        finished_request(fd, current->get_serv_index());
				}
				else if (_fd_map[fd].req.find(transferEncodingStr) != std::string::npos) {
				    if (_fd_map[fd].req.substr(_fd_map[fd].req.length()-5) == "0\r\n\r\n")
				        finished_request(fd, current->get_serv_index());
				}	
			}
			else if (filter == EVFILT_WRITE)
			{
				if (_fd_map[fd].chunked == false)
				{
					ret = current->giveResponse(fd, _str_rep);
					if (ret == -1)
					{
						std::cout << BLUE << "Error sending to client (fd : " << fd << ") closed connection" << BLUE_B << std::endl;
						close_connection(fd, current);
						return ;
					}
					if (ret == 0)
					{
						if (_str_rep.size() > 0)
						{
							std::cout << BLUE << "Error sending to client (fd : " << fd << "), closing connection" << BLUE_B << std::endl;
							close_connection(fd, current);
							return ;
						}		
					}
				}
				else
				{
					ret = current->giveResponseChunked(fd, &_fd_map[fd]);
					if (ret == -1)
					{
						std::cout << BLUE << "Error sending to client (fd : " << fd << ") closed connection" << BLUE_B << std::endl;
						close_connection(fd, current);
						return ;
					}
					else if ((ret + _fd_map[fd].byte_sent) <= _fd_map[fd].chunks[_fd_map[fd].chunk_sent].size())
					{
						_fd_map[fd].chunk_sent++;
						_fd_map[fd].byte_sent = 0;

						if (_fd_map[fd].chunk_sent == _fd_map[fd].chunks.size())
						{
							std::cout << BLUE << "Write on client (fd : " << fd << ")" << BLUE_B << std::endl;
							clear_response(fd);
							break;
						}
						set_write_event(fd);
					}
					else if ((ret + _fd_map[fd].byte_sent)  < _fd_map[fd].chunks[_fd_map[fd].chunk_sent].size())
					{
						_fd_map[fd].byte_sent += ret;
						set_write_event(fd);
					}
				}
				if (_fd_map[fd].close)
				{
					std::cout << BLUE << "Connection on client (fd : " << fd  << ") closed" << BLUE_B << std::endl;
					close_connection(fd, current);
				}
			}
		}
	}
	usleep(5000);
}

void WebServer::close_connection(int fd, ServerSocket *current)
{
	current->shrink_socket_clients(fd);
	shrink_kqueue_fd(fd);
	clear_response(fd);
	close(fd);
}

void WebServer::set_write_event(int fd)
{
	struct kevent ev = _fd_map[fd].events;
	EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
	_fd_map[fd].events = ev;
	if (kevent(_kq, &ev, 1, NULL, 0, NULL) == -1)
		std::cerr << "Error in kevent" << std::strerror(errno) << std::endl;
}

void WebServer::finished_request(int fd, size_t serv_index)
{
	std::cout << BLUE << "\nRead on client (fd : " << fd << ")" << BLUE_B << std::endl;
	handle_client(fd, serv_index);
	_fd_map[fd].req.clear();
	set_write_event(fd);
}

void WebServer::clear_response(int fd)
{
	_fd_map[fd].chunks.clear();
	_fd_map[fd].chunked = false;
	_fd_map[fd].byte_sent = 0;
	_fd_map[fd].chunk_sent = 0;
}

void	WebServer::shrink_kqueue_fd(int fd)
{
	struct kevent ev;
  	EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, 0);
  	if (kevent(_kq, &ev, 1, NULL, 0, NULL) == -1) {
    	std::cerr << "Error deleting socket from kqueue: " << std::strerror(errno) << std::endl;
  	}
}

std::vector<std::string> WebServer::chunk_message(int fd, std::string *message)
{

		std::string header_name = "Content-Length";
		size_t header_pos = message->find(header_name);
		if (header_pos != std::string::npos) {
			message->erase(header_pos, message->find("\r\n", header_pos) - header_pos + 2);
		}
		message->insert(message->find("\r\n", message->find("\r\n")) + 2, "Transfer-Encoding: chunked\r\n");
		
		std::cout << YELLOW_B << "Response: " << std::endl << YELLOW << getHttpRequestWithoutBody(*message).c_str() << NONE << std::endl << std::endl;

		size_t sent = 0;
		bool first_chunk = true;
		std::vector<std::string> chunks;
		
		while (sent < message->length()) {
			int to_send = std::min((int)_chunk_size, (int)(message->length() - sent));
			std::stringstream ss;
			std::string chunk;

			if (first_chunk) {
				size_t headers_end = message->find("\r\n\r\n");
				ss << std::hex << to_send - (headers_end + 4);
				std::string _chunk_size = ss.str();
				chunk = message->substr(0, headers_end + 4) + _chunk_size + "\r\n" + message->substr(headers_end + 4, (to_send - (headers_end + 4))) + "\r\n";
				first_chunk = false;
			}
			else {
				ss << std::hex << to_send;
				std::string _chunk_size = ss.str();
				chunk = _chunk_size + "\r\n" + message->substr(sent, to_send) + "\r\n";
			}
			chunks.push_back(chunk);
			sent += to_send;
		}
		chunks.push_back("0\r\n\r\n");
		_fd_map[fd].chunked = true;
		return chunks;
}

void	WebServer::handle_client(int fd, size_t serv_index)
{
	HandleHttp	handle(_fd_map[fd].req, _config, serv_index);

	handle.show_request();
	handle.do_work();

	_fd_map[fd].close = handle.client_close();
	_str_rep = handle.get_response().give_response();
	if (_str_rep.size() > _chunk_size && _fd_map[fd].chunked == false)
		_fd_map[fd].chunks = chunk_message(fd, &_str_rep);
}
