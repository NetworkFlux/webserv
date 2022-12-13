#pragma once

#include "../utils.hpp"

class SimpleConfig
{
	// Member Variables
	public:
		std::string							_listen;
		std::vector<std::string>			_serverNames;
		std::map<int, std::string>			_errorPages;
		// int									_maxBody;
		// std::string							_root;
		std::string							_autoIndex;
		std::vector<std::string>			_index;
		// std::map<std::string, std::string>	_fastcgi;

	// Constructors
	public:
		SimpleConfig();
		~SimpleConfig();

};