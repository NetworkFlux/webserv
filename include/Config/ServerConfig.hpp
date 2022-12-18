#pragma once

#include <fstream>

#include "SimpleConfig.hpp"

/*	USAGE: To use the ServerConfig class you must pass the path to the configuration
	file as agrument of its constructor. Its constructor will create, parse and configure
	everything that is needed to work later. --ServerConfig	name(path_to_conf)--
	Because there can be multiple server, we'll have a vector of SimpleConfig. One
	SimpleConfig for each server needed.	*/
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

	// Helper Functions
	private:
		std::vector<std::string>	splitLine(const std::string& line, const std::string& sep) const;
		long						convertToByte(const std::string& value);

};