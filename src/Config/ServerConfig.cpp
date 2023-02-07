#include "../../include/Config/ServerConfig.hpp"

/*	This constructor will parsed the configuration file and store
*	all its informations in the ServerConfig class making its
*	data usable for use.
*/
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
			{
				std::vector<std::string> split2;
				split2 = splitLine(split[i], ":");
				if (split2[1].empty())
				{
					std::cout << "Bad ip:port format" << std::endl;
					exit(EXIT_FAILURE);
				}
				simple->_listen.insert(std::make_pair(split2[0], split2[1]));
			}
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
		else if (line.find("redirect") != std::string::npos)
		{
			split = splitLine(line, " ;\t");
			simple->_redirect = split[1];
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
{}

/*	This function will output all the data stored in a ServerConfig object,
*	usefull for troubleshooting. It outputs the data for each server.
*/
void	ServerConfig::printInfo(void)
{
	// Print Title
	std::cout << YELLOW_B <<"-- SERVER CONFIGURATION --" << NONE << std::endl << std::endl;
	for (size_t i = 0; i < _servConf.size(); i++)
	{
		std::cout << GREEN_B << "\t| Server " << (i + 1) << " |" << std::endl;
		std::cout << "\t------------" << std::endl;
		_servConf[i].printConf("", RED);
		for (std::map<std::string, SimpleConfig>::const_iterator it = _servConf[i]._locations.begin(); it != _servConf[i]._locations.end(); ++it)
		{
			std::cout << PURPLE_B << "locations : " << PURPLE << it->first << std::endl;
			it->second.printConf("\t", PURPLE);
		}
		std::cout << std::endl;
	}
}