#include "../include/Config/Config.hpp"

Config::Config(std::string path) : confPath(path), port(), host(), errorPage()
{
	std::string	line;

	_confFile.open(confPath);
	while (getline(_confFile, line))
		std::cout << line << std::endl;
	_confFile.close();
}

Config::~Config() {}