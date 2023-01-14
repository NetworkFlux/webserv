#pragma once

#include "Config/ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "HandleHttp.hpp"

class WebServer
{
	// Private Variables
	private:
		struct FdInfo
		{
			struct kevent events;
			bool close;
			std::string req;
		};

		ServerConfig*					_config;	// Config object
		std::vector<ServerSocket*>		_sockets_list;	// List of all the sockets
		std::map<int, FdInfo> 			_fd_map;		// Map that lists active event for each fd
		int 							_kq;			// FD of the kqueue	
		std::string						_str_req;	// String request
		std::string						_str_rep;	//	String reply

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
		void	handle_client(int fd, size_t serv_index);
};