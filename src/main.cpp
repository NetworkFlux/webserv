#include "../include/Socket/ServerSocket.hpp"
#include "../include/Socket/ClientSocket.hpp"
#include "../include/Config/Config.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Bad file." << std::endl;
		return (1);
	}

	Config	config(argv[1]);

	return (0);
}
 