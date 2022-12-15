#include "../../include/Config/SimpleConfig.hpp"

/*	This constructor only initialize the variable for later use.	*/
SimpleConfig::SimpleConfig() : _listen(), _serverNames(), _errorPages(), _index(), _root(),
	_maxBody(0), _methods(), _uploadPath(), _cgiExt(), _cgiPath(), _locations()
{
	_autoIndex = false;
	_upload = false;
}

/*	Deletes the values stored in the member variable after some necessary steps.	*/
SimpleConfig::~SimpleConfig()
{

}

/*	This function will output all the data stored in a SimpleConfig object, usefull for
	troubleshooting. A prefix can be used for a nicer output managment aswell as
	specific color.	*/
void	SimpleConfig::printConf(std::string prefix, std::string color) const
{
	std::string	upPrefix;

	upPrefix.append(prefix);
	// Listen
	if (_listen.size())
	{
		std::cout << color << upPrefix << "listen \t\t: " << color ;
		for (size_t j = 0; j < _listen.size(); j++)
			std::cout << _listen[j] << " - ";
		std::cout << RESET << std::endl;
	}
	// Server names
	if (_serverNames.size())
	{
		std::cout << color << upPrefix << "server_name \t: " << color;
		for (size_t j = 0; j <_serverNames.size(); j++)
			std::cout << _serverNames[j] << " - ";
		std::cout << RESET << std::endl;
	}
	// Error pages
	if (_errorPages.size())
	{
		std::cout << color << upPrefix << "error_pages \t: " << color;
		for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
			std::cout << "(" << it->first << " -> " << it->second << ") - ";
		std::cout << RESET << std::endl;
	}
	// Autoindex
	if (_autoIndex)
	{
		std::cout << color << upPrefix << "autoindex \t: " << color;
		std::cout << _autoIndex << RESET << std::endl;
	}
	// Index
	if (_index.size())
	{
		std::cout << color << upPrefix << "index \t\t: " << color;
		for (size_t j = 0; j < _index.size(); j++)
			std::cout << _index[j] << " - ";
		std::cout << RESET << std::endl;
	}
	// Root
	if (!_root.empty())
	{
		std::cout << color << upPrefix << "root \t\t: " << color;
		std::cout << _root << RESET << std::endl;
	}
	// Max body size
	if (_maxBody)
	{
		std::cout << color << upPrefix << "client_max_size\t: " << color;
		std::cout << _maxBody << RESET << std::endl;
	}
	// Methods
	if (_methods.size())
	{
		std::cout << color << upPrefix << "methods \t: " << color;
		for (size_t j = 0; j < _methods.size(); j++)
			std::cout << _methods[j] << " - ";
		std::cout << RESET << std::endl;
	}
	// Upload
	if (_upload)
	{
		std::cout << color << upPrefix << "upload \t\t: " << color;
		std::cout << _upload << RESET << std::endl;
	}
	// Upload path
	if (!_uploadPath.empty())
	{
		std::cout << color << upPrefix << "upload path \t: " << color;
		std::cout << _uploadPath << RESET << std::endl;
	}
	// CGI extentions
	if (_cgiExt.size())
	{
		std::cout << color << upPrefix << "CGI extentions \t: " << color;
		for (size_t j = 0; j < _cgiExt.size(); j++)
			std::cout << _cgiExt[j] << " - ";
		std::cout << RESET << std::endl;
	}
	// CGI path
	if (!_cgiPath.empty())
	{
		std::cout << color << upPrefix << "CGI path \t: " << color;
		std::cout << _cgiPath << RESET << std::endl;
	}
	std::cout << std::endl;
}