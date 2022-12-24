#pragma once

#include "Config/ServerConfig.hpp"
#include "../include/Socket/ServerSocket.hpp"
#include "../include/Http/HandleHttp.hpp"

class WebServer
{
	// Private attrbutes
	private:
		std::vector<ServerSocket*>		_sockets_list;
		struct pollfd					_fds[200];
		size_t 							_nfds;
		bool							_close;
		std::string 					_str_req;
		std::string 					_str_rep;
		
		// bool _compress_pollFds;

	// Constructors
	public:
		WebServer();
		~WebServer();

	// Member Functions
		void			createServers(ServerConfig &config);
		void 			runServers();;
		void 			handleServer(int ind);
		void			shrink_poll_fd(int fd);
		void			handle_client();
};
