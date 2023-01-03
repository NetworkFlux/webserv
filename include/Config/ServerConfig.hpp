#pragma once

#include "SimpleConfig.hpp"

class ServerConfig
{
	// Member Variables
	public:
		std::vector<SimpleConfig>	_servConf;
		std::ifstream				_file;

	// Constructors
	public:
		ServerConfig(std::string path);
		~ServerConfig();

	// Member Function
	public:
		void	printInfo(void);
};