#include "../../include/Config/ServerConfig.hpp"

/*	This constructor will parsed the configuration file and store all its informations
	in the ServerConfig class making its data usable for use.	*/
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

/*	This functions converts a string like "15G" to the number of bytes in 15 GigaBytes.	*/
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

/*	This function will output all the data stored in a ServerConfig object, usefull for
	troubleshooting. It outputs the data for each server.	*/
void	ServerConfig::printInfo(void)
{
	// Print Title
	std::cout << YELLOW_B <<"-- SERVER CONFIGURATION --" << RESET << std::endl << std::endl;
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