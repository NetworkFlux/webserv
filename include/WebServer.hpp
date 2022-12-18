#pragma once

#include "Config/ServerConfig.hpp"
#include "../include/Socket/ServerSocket.hpp"
#include "../include/Http/HandleHttp.hpp"

class WebServer
{
	// Constructors
	public:
		WebServer();
		~WebServer();

	// Member Functions
		void		listen(ServerConfig *config);
		std::string	handle_client(std::string req);
};