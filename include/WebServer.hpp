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
		std::map<int, FdInfo> 			_fd_map;		// Map that lists active event for each fd
		int 							_kq;			// FD of the kqueue
		std::string						_str_req;	// String request
		std::string						_str_rep;	//	String reply
		size_t 							_chunk_size;

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
		void 	finished_request(int fd, size_t serv_index);
		void 	clear_response(int fd);
		void 	set_write_event(int fd);
		void	close_connection(int fd, ServerSocket *current);
		std::vector<std::string> chunk_message(int fd, std::string *message);
};