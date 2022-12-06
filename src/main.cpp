#include "../include/Socket/ServerSocket.hpp"
#include "../include/Socket/ClientSocket.hpp"

int	main(void)
{
	ServerSocket	server(AF_INET, SOCK_STREAM, 0, 19000, INADDR_ANY);

	server.listening(10);
	server.grabConnection();
	return (0);
}
