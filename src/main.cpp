#include "../include/Socket/ServerSocket.hpp"
#include "../include/Socket/ClientSocket.hpp"
#include "../include/Config/ServerConfig.hpp"
#include "../include/WebServer.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Bad file." << std::endl;
		return (1);
	}

	ServerConfig	config(argv[1]); // Easy way to launch the parsing
	WebServer		web_server; // Create  WebServer with all the servers configs

	web_server.listen(&config);
	
	return (0);
}
