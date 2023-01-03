#pragma once

#include "utils.hpp"

class ServerSocket
{
	// Member Variables
	private:
		int					_sockFD; // File Descriptor to the Socket
		sockaddr_in			_address;
		int					_addressLen;
		int					_connection; // File Descriptor to the established connection
		char				_buffer[BUFFER_SIZE];
		std::string 		_request;
		std::vector<int> 	_socket_clients;

	// Constructors
	public:
		ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface);
		~ServerSocket();
	
	// Member Functions
	public:
		void				listeningMode(int maxIncoming);
		void				grabConnection(void);
		std::string			readConnection(struct pollfd *ptr_tab_poll);
		int					giveResponse(struct pollfd *ptr_tab_poll, std::string message);
		void 				socketConf();
		std::vector<int>&	get_socket_client();
		void 				shrink_socket_clients(int to_find);
		int					get_sock_fd(void);
};