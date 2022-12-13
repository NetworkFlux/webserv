#pragma once

#include <fstream>

#include "SimpleConfig.hpp"

/*	USAGE: To use the ServerConfig class you must pass the path to the configuration
	file as agrument of its constructor. Its constructor will create, parse and configure
	everything that is needed to work later. --ServerConfig	name(path_to_conf)-- */
class ServerConfig
{
	// Member Variables
	private:
		std::vector<SimpleConfig>	_servConf;
		std::ifstream				_file;

	// Constructors
	public:
		ServerConfig(std::string path);
		~ServerConfig();

	// Member Function
	public:
		void	printConf(void) const;

	// Helper Functions
	private:
		std::vector<std::string>	splitLine(const std::string& line, const std::string& sep) const;

};