#pragma once

#include "Config/ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "HandleHttp.hpp"

class WebServer
{
	// Private Variables
	private:
		ServerConfig*					_config;	// Config object
		std::vector<ServerSocket*>		_sockets_list;	// List of all the sockets
		std::map<int, struct kevent> 	_events;		// Map that lists active event for each fd
		int 							_kq;			// FD of the kqueue	
		std::string						_str_req;	// String request
		std::string						_str_rep;	//	String reply
		bool							_close;

	// Constructors
	public:
		WebServer(ServerConfig& config);
		~WebServer();

	// Member Functions
	public:
		void	printInfo(void);
		void	createServers(void);
		void 	runServers(void);
		void	handleServer(int fd, int filter);
		void	shrink_kqueue_fd(int fd);
		void	handle_client(size_t serv_index);
};