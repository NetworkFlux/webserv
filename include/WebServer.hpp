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
		struct pollfd					_fds[OPEN_MAX];	// List of all the fds
		size_t							_nfds;		// Number of fds
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
		void	handleServer(int index);
		void	shrink_poll_fd(int fd);
		void	handle_client(void);
};