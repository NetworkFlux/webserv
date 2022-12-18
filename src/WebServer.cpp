#include "../include/WebServer.hpp"

WebServer::WebServer(void)
{}

WebServer::~WebServer(void)
{}

/* This class should change to be able to 
1/ accept multiple connection at the same time
2/ stop a user connection if header field Connection:keep-alive is not set */

/* Waits for user connecting */
void	WebServer::listen(ServerConfig *config)
{
	ServerSocket	server_socket(AF_INET, SOCK_STREAM, 0, stoi(config->_servConf[0]._listen[0]), INADDR_ANY);

	server_socket.listeningMode(5);
	for ( ; ; )
	{
		server_socket.grabConnection();
		server_socket.giveResponse( handle_client(server_socket.readConnection()) );
	}
}

std::string	WebServer::handle_client(std::string str_request)
{
	HandleHttp *handl = new HandleHttp();
	std::string ret;

	ret = handl->do_work(str_request);

	delete(handl);
	return (ret);
}