#pragma once

#include "FdInfo.hpp"
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
		size_t				_serv_index;


	// Constructors
	public:
		ServerSocket(int domain, int type, int protocol, int port, u_int32_t interface, size_t serv_index);
		~ServerSocket();

	public:
		size_t	get_serv_index();

	// Member Functions
	public:
		void				listeningMode(int maxIncoming);
		int					grabConnection(void);
		int					readConnection(int fd, std::string *req);
		int					giveResponse(int fd, std::string message);
		// int					giveResponseChunked(int fd, std::vector<std::string>,  size_t *chunk_sent, size_t *byte_sent);
		int					giveResponseChunked(int fd, FdInfo*);
		void 				socketConf();
		std::vector<int>&	get_socket_client();
		void 				shrink_socket_clients(int to_find);
		int					get_sock_fd(void);
};
