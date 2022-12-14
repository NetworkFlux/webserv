#pragma once

#include "../utils.hpp"

class SimpleConfig
{
	// Member Variables
	public:
		std::vector<std::string>			_listen;		// List of where to listen to connections
		std::vector<std::string>			_serverNames;	// List of possible server names
		std::map<int, std::string>			_errorPages;	// Maping of errors and their path
		bool								_autoIndex;		// Autoindex on or off
		std::vector<std::string>			_index;			// List of possible index
		std::string							_root;			// Path to root
		long									_maxBody;		// Max length to be converted in bytes
		std::vector<std::string>			_methods;		// Allowed methods
		bool								_upload;		// Is upload on or off
		std::string							_uploadPath;	// Upload path
		std::vector<std::string>			_cgiExt;		// List of CGI extentions
		std::string							_cgiPath;		// CGI path
		std::map<std::string, SimpleConfig>	_locations;		// Maping of locations and their new propreties

	// Constructors
	public:
		SimpleConfig();
		~SimpleConfig();

};