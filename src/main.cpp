#include "../include/Socket/ServerSocket.hpp"
#include "../include/Socket/ClientSocket.hpp"
#include "../include/Config/ServerConfig.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Bad file." << std::endl;
		return (1);
	}

	ServerConfig	config(argv[1]); // Easy way to launch the parsing
	ServerSocket	server(AF_INET, SOCK_STREAM, 0, stoi(config._servConf[0]._listen[0]), INADDR_ANY);
	server.listeningMode(5);
	server.grabConnection();
	return (0);
}
