#include "../include/WebServer.hpp"

WebServer::WebServer(ServerConfig& config) : _sockets_list(), _str_req(), _str_rep()
{
	_config = &config;
	_nfds = 0;
	_close = false;
}

WebServer::~WebServer()
{
}

void	WebServer::printInfo(void)
{
	_config->printInfo();
}

/* Create all server instances */
void	WebServer::createServers(void)
{
	for (; _nfds < _config->_servConf.size(); _nfds++)
	{
		ServerSocket* s = new ServerSocket(AF_INET, SOCK_STREAM, 0, stoi(_config->_servConf[_nfds]._listen[0]), INADDR_ANY, _nfds); // index 0 should not stay like that
		s->socketConf();
		s->listeningMode(5);
		_fds[_nfds].fd = s->get_sock_fd();
		_fds[_nfds].events = POLLIN;
		_sockets_list.push_back(s);
	}
}

/* Create the poll and loop through the socket to monitor events */
void WebServer::runServers()
{
	int ret;
	int timeout = (3 * 60 * 1000);

	while (true)
	{
		ret = poll(_fds, _nfds, timeout);
		if (ret == -1) {
			std::cout << "poll failed" << std::endl;
			return ;
		}
		if (ret == 0) {
			std::cout << "poll timed out" << std::endl;
			return ;
		}
		for (size_t index = 0; index < _nfds; index++)
		{
			if (_fds[index].revents == 0)
				continue;
			handleServer(index);
		}
	}
}

/* This function goes through every socket for each ports and for each
 1/ If a new connection came, add it their client vector and to the poll
 2/ It it is an existing socket, read or send */
void WebServer::handleServer(int index)
{
	int		old_size;
	int		ret;


	for (std::vector<ServerSocket*>::iterator it = _sockets_list.begin(); it != _sockets_list.end(); it++)
	{
		ServerSocket *current = *it.base();
		std::vector<int> & current_socket_client = current->get_socket_client();
		old_size = current_socket_client.size();

		if (_fds[index].fd == current->get_sock_fd())
		{
			current->grabConnection();
			for (size_t i = old_size; i < current_socket_client.size(); i++)
			{
				_fds[_nfds].fd = current_socket_client[i];
				_fds[_nfds].events = POLLIN;
				_fds[_nfds].revents = 0;
				_nfds++;
			}

		}
		else if ((std::find(current_socket_client.begin(), current_socket_client.end(), _fds[index].fd)) != current_socket_client.end())
		{
			_close = false;
			if (_fds[index].revents & POLLOUT)
			{
				ret = current->giveResponse(&_fds[index], _str_rep);
				if (ret < 0)
					std::cout << "send() failed!" << std::endl;
				// else if (ret == 0)
				// 	_close = true;
			}
			else if ((_str_req = current->readConnection(&_fds[index])).size() > 0)
			{
				handle_client(current->get_serv_index());
				_fds[index].events = POLLOUT;
			}
			if (_close)
			{
				std::cout << "close connection pollFd = " << _fds[index].fd << "\n" <<std::endl;
				close(_fds[index].fd);
				current->shrink_socket_clients(_fds[index].fd);
				shrink_poll_fd(_fds[index].fd);
			}
		}
	}
}

void	WebServer::shrink_poll_fd(int fd)
{
	for (size_t i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd == fd)
		{
			for(size_t j = i; j < _nfds - 1; j++)
			{
				_fds[j].fd = _fds[j+1].fd;
			}
			i--;
			_nfds--;
		}
	}
}

void	WebServer::handle_client(size_t serv_index)
{
	HandleHttp	handle(get_first_line(_str_req), _config, serv_index);
	handle.show_request();
	handle.do_work();
	_str_rep = handle.get_response().give_response();
}
