#include "../include/WebServer.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Bad file." << std::endl;
		return (1);
	}

	ServerConfig	config(argv[1]); // Easy way to launch the parsing
	WebServer		web_server(config); // Create WebServer with all the servers configs
	web_server.printInfo(); // Print all the servers configs
	web_server.createServers(); // Create all the servers
	web_server.runServers(); // Run all the servers
	return (0);
}