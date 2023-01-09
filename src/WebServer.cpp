#include "../include/WebServer.hpp"

WebServer::WebServer(ServerConfig& config) : _sockets_list(), _str_req(), _str_rep()
{
	_config = &config;
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
	_kq = kqueue();
	if (_kq == -1) {
  		std::cerr << "Error creating kqueue: " << std::strerror(errno) << '\n';
  		return;
	}

	for (size_t j = 0; j < _config->_servConf.size(); j++)
	{
		for (size_t i = 0; i < _config->_servConf[j]._listen.size(); i++)
		{
			ServerSocket* s = new ServerSocket(AF_INET, SOCK_STREAM, 0, stoi(_config->_servConf[j]._listen[i]), INADDR_ANY, j); // index 0 should not stay like that
			s->socketConf();
			s->listeningMode(5);

			struct kevent ev;
    		EV_SET(&ev, s->get_sock_fd(), EVFILT_READ, EV_ADD, 0, 0, nullptr);
			_events[s->get_sock_fd()] = ev;
    		if (kevent(_kq, &ev, 1, nullptr, 0, nullptr) == -1) {
      			std::cerr << "Error adding event to kqueue: " << std::strerror(errno) << '\n';
      			return;
    		}

			_sockets_list.push_back(s);
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
		if (n == -1) {
			std::cerr << "Error waiting for events: " << std::strerror(errno) << std::endl;
			return;
		}
		// Process the events
		for (int i = 0; i < n; ++i)
		{
			int fd = evlist[i].ident;
			int filter = evlist[i].filter;

			if (filter == EVFILT_READ || filter == EVFILT_WRITE) {
				std::cout << "Event on fd : " << fd << " of type " << filter << std::endl;
				handleServer(fd, filter);
			}
		}
	}
}


/* This function goes through every socket for each ports and for each
 1/ If a new connection came, grab it and add it to the kqueue
 2/ It it is an existing socket, read or send and change the event triggering */
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
        		_events[current_socket_client[i]] = event;
				if (kevent(_kq, &event, 1, NULL, 0, NULL) == -1) {
          			std::cerr << "Error adding socket to kqueue: " << std::strerror(errno) << std::endl;
        		}
			}
		}
		else if (std::find(current_socket_client.begin(), current_socket_client.end(), fd) != current_socket_client.end())
		{
			_close = false;
			if (filter == EVFILT_READ)
			{
				ret = current->readConnection(fd, &_str_req);
				if (ret == 0)
				 	_close = true;
        		handle_client(current->get_serv_index());
				struct kevent ev = _events[fd];
				EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, 0);
				_events[fd] = ev;
				if (kevent(_kq, &ev, 1, NULL, 0, NULL) == -1) {
					std::cerr << "Error in kevent" << std::strerror(errno) << std::endl;
				}
			}
			else if (filter == EVFILT_WRITE)
			{
				ret = current->giveResponse(fd, _str_rep);
			 	if (ret < 0)
			 		std::cout << "send() failed!" << std::endl;
				struct kevent ev = _events[fd];
				EV_SET(&ev, fd, EVFILT_READ, EV_ENABLE, 0, 0, 0);
				_events[fd] = ev;
				if (kevent(_kq, &ev, 1, NULL, 0, NULL) == -1) {
					std::cerr << "Error in kevent" << std::strerror(errno) << std::endl;
				}
			}
      		if (_close)
			{
        		std::cout << "close connection fd = " << fd << std::endl;
				close(fd);
				current->shrink_socket_clients(fd);
				shrink_kqueue_fd(fd);
			}
		}
	}
}

void	WebServer::shrink_kqueue_fd(int fd)
{
	struct kevent ev;
  	EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, 0);
  	if (kevent(_kq, &ev, 1, NULL, 0, NULL) == -1) {
    	std::cerr << "Error deleting socket from kqueue: " << std::strerror(errno) << std::endl;
  	}
}

void	WebServer::handle_client(size_t serv_index)
{
	HandleHttp	handle(_str_req, _config, serv_index);
	handle.show_request();
	handle.do_work();
	_str_rep = handle.get_response().give_response();
}
