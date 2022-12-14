#include "../../include/Config/ServerConfig.hpp"

/*	This constructor will parsed the configuration file and store all its informations
	in the ServerConfig class makings its data usable for use.	*/
ServerConfig::ServerConfig(std::string path) : _servConf(), _file(path)
{
	std::string	line;
	std::vector<std::string> split;
	int		status = 0; // 0 = global scope; 1 = server scope; 2 = location scope;
	SimpleConfig*	simple;

	while (getline(_file, line))
	{
		if (line.front() == '#')
			continue;
		if (status < 2)
			simple = &_servConf[_servConf.size() - 1];
		if (line.find("server {") != std::string::npos)
		{
			_servConf.push_back(SimpleConfig());
			status = 1;
		}
		else if (line.find("location ") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			_servConf[_servConf.size() - 1]._locations.insert(std::make_pair(split[1], SimpleConfig()));
			status = 2;
			simple = &_servConf[_servConf.size() - 1]._locations[split[1]];
		}
		else if (line.find("}") != std::string::npos)
			status--;
		else if (line.find("listen ") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				simple->_listen.push_back(split[i]);
		}
		else if (line.find("server_name ") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				simple->_serverNames.push_back(split[i]);
		}
		else if (line.find("error_page ") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_errorPages.insert(std::make_pair(stoi(split[1]), split[2]));

		}
		else if (line.find("index") != std::string::npos)
		{
			if (line.find("autoindex") != std::string::npos && line.find("on") != std::string::npos)
				simple->_autoIndex = true;
			else if (line.find("autoindex") != std::string::npos)
				continue;
			else
			{
				split = splitLine(line, " ;\t");
				for (size_t i = 1; i < split.size(); i++)
					simple->_index.push_back(split[i]);
			}
		}
		else if (line.find("root") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_root = split[1];
		}
		else if (line.find("client_max_body_size") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_maxBody = convertToByte(split[1]);
		}
		else if (line.find("method") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				simple->_methods.push_back(split[i]);
		}
		else if (line.find("upload_enable") != std::string::npos && line.find("on") != std::string::npos)
			simple->_upload = true;
		else if (line.find("upload_path") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_uploadPath = split[1];
		}
		else if (line.find("cgi_extension") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				simple->_cgiExt.push_back(split[i]);
		}
		else if (line.find("cgi_path") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_cgiPath = split[1];
		}
	}
}

/* The destructor does not need to clean anything right now.	*/
ServerConfig::~ServerConfig()
{

}

/*	This function will split a string into its individual words enabeling easy parsing.	*/
std::vector<std::string>	ServerConfig::splitLine(const std::string& line, const std::string& sep) const
{
	std::vector<std::string>	res;
	std::string	temp;
	size_t	i = 0;

	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (i < line.length())
	{
		if (charInStr(line[i], sep) && temp.length() != 0)
		{
			res.push_back(temp);
			temp.clear();
		}
		else
			temp.append(1, line[i]);
		i++;
	}
	return (res);
}

long		ServerConfig::convertToByte(const std::string& value)
{
	long	res = 0;
	std::string	unit;

	for (size_t i = 0; i < value.size(); i++)
	{
		if (isdigit(value[i]))
			res = (res * 10) + (value[i] - 48);
		else
			unit.append(1, value[i]);
	}
	std::cout << res;
	if (unit == "G")
		return (res * 1073741824);
	else if (unit == "k")
		return (res * 1024);
	else if (unit == "M")
		return (res * 1048576);
	else
		return (res);
}

/*	This function will output all the data stored in the ServerConf class, usefull for
	troubleshooting. It outputs the data for each server.	*/
void	ServerConfig::printConf(void) const
{
	// Print Title
	std::cout << YELLOW_B <<"-- SERVER CONFIGURATION --" << RESET << std::endl << std::endl;
	for (size_t i = 0; i < _servConf.size(); i++)
	{
		std::cout << GREEN_B << "\t| Server " << (i + 1) << " |" << std::endl;
		std::cout << "\t------------" << std::endl;
		// Listen
		if (_servConf[i]._listen.size())
		{
			std::cout << RED_B << "listen \t\t: " << RED ;
			for (size_t j = 0; j < _servConf[i]._listen.size(); j++)
				std::cout << _servConf[i]._listen[j] << " - ";
			std::cout << RESET << std::endl;
		}
		// Server names
		if (_servConf[i]._serverNames.size())
		{
			std::cout << RED_B << "server_name \t: " << RED;
			for (size_t j = 0; j < _servConf[i]._serverNames.size(); j++)
				std::cout << _servConf[i]._serverNames[j] << " - ";
			std::cout << RESET << std::endl;
		}
		// Error pages
		if (_servConf[i]._errorPages.size())
		{
			std::cout << RED_B << "error_pages \t: " << RED;
			for (std::map<int, std::string>::const_iterator it = _servConf[i]._errorPages.begin(); it != _servConf[i]._errorPages.end(); ++it)
				std::cout << "(" << it->first << " -> " << it->second << ") - ";
			std::cout << RESET << std::endl;
		}
		// Autoindex
		if (_servConf[i]._autoIndex)
		{
			std::cout << RED_B << "autoindex \t: " << RED;
			std::cout << _servConf[i]._autoIndex << RESET << std::endl;
		}
		// Index
		if (_servConf[i]._index.size())
		{
			std::cout << RED_B << "index \t\t: " << RED;
			for (size_t j = 0; j < _servConf[i]._index.size(); j++)
				std::cout << _servConf[i]._index[j] << " - ";
			std::cout << RESET << std::endl;
		}
		// Root
		if (!_servConf[i]._root.empty())
		{
			std::cout << RED_B << "root \t\t: " << RED;
			std::cout << _servConf[i]._root << RESET << std::endl;
		}
		// Max body size
		if (_servConf[i]._maxBody)
		{
			std::cout << RED_B << "client_max_size\t: " << RED;
			std::cout << _servConf[i]._maxBody << RESET << std::endl;
		}
		// Methods
		if (_servConf[i]._methods.size())
		{
			std::cout << RED_B << "methods \t: " << RED;
			for (size_t j = 0; j < _servConf[i]._methods.size(); j++)
				std::cout << _servConf[i]._methods[j] << " - ";
			std::cout << RESET << std::endl;
		}
		// Upload
		if (_servConf[i]._upload)
		{
			std::cout << RED_B << "upload \t\t: " << RED;
			std::cout << _servConf[i]._upload << RESET << std::endl;
		}
		// Root
		if (!_servConf[i]._uploadPath.empty())
		{
			std::cout << RED_B << "upload path \t: " << RED;
			std::cout << _servConf[i]._uploadPath << RESET << std::endl;
		}
		// CGI extentions
		if (_servConf[i]._cgiExt.size())
		{
			std::cout << RED_B << "CGI extentions \t: " << RED;
			for (size_t j = 0; j < _servConf[i]._cgiExt.size(); j++)
				std::cout << _servConf[i]._cgiExt[j] << " - ";
			std::cout << RESET << std::endl;
		}
		// CGI path
		if (!_servConf[i]._cgiPath.empty())
		{
			std::cout << RED_B << "CGI path \t: " << RED;
			std::cout << _servConf[i]._cgiPath << RESET << std::endl;
		}
		for (std::map<std::string, SimpleConfig>::const_iterator it = _servConf[i]._locations.begin(); it != _servConf[i]._locations.end(); ++it)
		{
			std::cout << PURPLE_B << "locations : " << PURPLE << it->first << std::endl;
			if (it->second._listen.size())
			{
				std::cout << PURPLE_B << "\tlisten \t\t: " << PURPLE ;
				for (size_t j = 0; j < it->second._listen.size(); j++)
					std::cout << it->second._listen[j] << " - ";
				std::cout << RESET << std::endl;
			}
			// Server names
			if (it->second._serverNames.size())
			{
				std::cout << PURPLE_B << "\tserver_name \t: " << PURPLE;
				for (size_t j = 0; j < it->second._serverNames.size(); j++)
					std::cout << it->second._serverNames[j] << " - ";
				std::cout << RESET << std::endl;
			}
			// Error pages
			if (it->second._errorPages.size())
			{
				std::cout << PURPLE_B << "\terror_pages \t: " << PURPLE;
				for (std::map<int, std::string>::const_iterator itb =  it->second._errorPages.begin(); itb != it->second._errorPages.end(); ++it)
					std::cout << "(" << itb->first << " -> " << itb->second << ") - ";
				std::cout << RESET << std::endl;
			}
			// Autoindex
			if (it->second._autoIndex)
			{
				std::cout << PURPLE_B << "\tautoindex \t: " << PURPLE;
				std::cout << it->second._autoIndex << RESET << std::endl;
			}
			// Index
			if (it->second._index.size())
			{
				std::cout << PURPLE_B << "\tindex \t\t: " << PURPLE;
				for (size_t j = 0; j < it->second._index.size(); j++)
					std::cout << it->second._index[j] << " - ";
				std::cout << RESET << std::endl;
			}
			// Root
			if (!it->second._root.empty())
			{
				std::cout << PURPLE_B << "\troot \t\t: " << PURPLE;
				std::cout << it->second._root << RESET << std::endl;
			}
			// Max body size
			if (it->second._maxBody)
			{
				std::cout << PURPLE_B << "\tclient_max_size\t: " << PURPLE;
				std::cout << it->second._maxBody << RESET << std::endl;
			}
			// Methods
			if (it->second._methods.size())
			{
				std::cout << PURPLE_B << "\tmethods \t: " << PURPLE;
				for (size_t j = 0; j < it->second._methods.size(); j++)
					std::cout << it->second._methods[j] << " - ";
				std::cout << RESET << std::endl;
			}
			// Upload
			if (it->second._upload)
			{
				std::cout << PURPLE_B << "\tupload \t\t: " << PURPLE;
				std::cout << it->second._upload << RESET << std::endl;
			}
			// Root
			if (!it->second._uploadPath.empty())
			{
				std::cout << PURPLE_B << "\tupload path \t: " << PURPLE;
				std::cout << it->second._uploadPath << RESET << std::endl;
			}
			// CGI extentions
			if (it->second._cgiExt.size())
			{
				std::cout << PURPLE_B << "\tCGI extentions \t: " << PURPLE;
				for (size_t j = 0; j < it->second._cgiExt.size(); j++)
					std::cout << it->second._cgiExt[j] << " - ";
				std::cout << RESET << std::endl;
			}
			// CGI path
			if (!it->second._cgiPath.empty())
			{
				std::cout << PURPLE_B << "\tCGI path \t: " << PURPLE;
				std::cout << it->second._cgiPath << RESET << std::endl;
			}
		}
		std::cout << std::endl;
	}
}