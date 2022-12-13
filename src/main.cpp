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
	config.printConf();
	return (0);
}
