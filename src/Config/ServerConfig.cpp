#include "../../include/Config/ServerConfig.hpp"

/*	This constructor will parsed the configuration file and store all its informations
	in the ServerConfig class makings its data usable for use.	*/
ServerConfig::ServerConfig(std::string path) : _servConf(), _file(path)
{
	std::string	line;

	while (getline(_file, line))
	{
		if (line.front() == '#')
			continue;
		if (line.find("server {") != std::string::npos)
			_servConf.push_back(SimpleConfig());
		if (line.find("listen ") != std::string::npos)
		{
			std::string	port;
			for (size_t i = 0; i < line.length(); i++)
			{
				if (isdigit(line[i]))
					port.append(1, line[i]);
			}
			_servConf[_servConf.size() - 1]._listen = port;
		}
		if (line.find("server_name ") != std::string::npos)
		{
			std::vector<std::string> split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				_servConf[_servConf.size() - 1]._serverNames.push_back(split[i]);
		}
		if (line.find("error_page ") != std::string::npos)
		{
			std::vector<std::string> split = splitLine(line, " ;\t");
			_servConf[_servConf.size() - 1]._errorPages.insert(std::make_pair(stoi(split[1]), split[2]));
		}
		if (line.find("autoindex") != std::string::npos)
		{
			std::vector<std::string> split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				_servConf[_servConf.size() - 1]._autoIndex = split[1];
		}
		else if (line.find("index") != std::string::npos)
		{
			std::vector<std::string> split = splitLine(line, " ;\t");
			for (size_t i = 1; i < split.size(); i++)
				_servConf[_servConf.size() - 1]._index.push_back(split[i]);
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
			// std::cout << temp << "--" << std::endl;
			temp.clear();
		}
		else
			temp.append(1, line[i]);
		i++;
	}
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
		std::cout << PURPLE_B << "listen \t\t: " << PURPLE << _servConf[i]._listen << RESET << std::endl;
		// Server names
		std::cout << RED_B << "server_name \t: " << RED;
		for (size_t j = 0; j < _servConf[i]._serverNames.size(); j++)
			std::cout << _servConf[i]._serverNames[j] << " ";
		std::cout << RESET << std::endl;
		// Error pages
		std::cout << RED_B << "error_pages \t: " << RED;
		for (std::map<int, std::string>::const_iterator it = _servConf[i]._errorPages.begin(); it != _servConf[i]._errorPages.end(); ++it)
			std::cout << "(" << it->first << " -> " << it->second << ") - ";
		std::cout << RESET << std::endl;
		// Autoindex
		std::cout << RED_B << "autoindex \t: " << RED;
		std::cout << _servConf[i]._autoIndex << RESET << std::endl;
		// Index
		std::cout << RED_B << "index \t\t: " << RED;
		for (size_t j = 0; j < _servConf[i]._index.size(); j++)
			std::cout << _servConf[i]._index[j] << " -- ";
		std::cout << RESET << std::endl;

		std::cout << std::endl;
	}
}