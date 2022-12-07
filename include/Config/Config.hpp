#pragma once

#include "ServerConfig.hpp"

class Config
{
	// Member Variables
	private:
		std::ifstream	_confFile;

	public:
		std::string	confPath;
		std::string	port;
		std::string	host;
		std::string	errorPage;
		//Location	location;


	// Unused Constructors
	private:
		Config();
		Config(const Config& src);

	// Constructors
	public:
		Config(std::string	path);
		~Config();

	// Member Functions

};