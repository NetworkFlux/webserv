#include "../../include/Methods/GetMethod.hpp"

/*	Initialize the member variable.	*/
GetMethod::GetMethod() : _errorCode(0)
{
	_body_content = "";
}

GetMethod::~GetMethod()
{}

/*	Returns the body content.	*/
const std::string&	GetMethod::get_body_content(void)
{
	return (_body_content);
}

/*	This fuction will make sure the ressources asked by the request are available
	and makes a response according to thoses asked ressources.	*/
void	GetMethod::do_method(HandleRequest* hand_req, HandleResponse* hand_resp, ServerConfig* config)
{
	std::string			path = "www";
	std::ifstream		ifs;
	std::stringstream	buffer;


	locationSetup(*config, &path, &ifs, hand_req);
	if (!errorCodeSetup(*config, &path, hand_resp))
		hand_resp->set_status_line("HTTP/1.1", "200", "OK");
	ifs.open(path);
	buffer << ifs.rdbuf();
	_body_content = buffer.str();
	if (_body_content.length())
	{
		hand_resp->make_header(_body_content);
		hand_resp->set_body(_body_content);
	}
}

void	GetMethod::locationSetup(ServerConfig& config, std::string* path, std::ifstream* ifs, HandleRequest* hand_req)
{
	std::map<std::string, SimpleConfig>::iterator it = config._servConf.front()._locations.find(hand_req->get_request().get_path());
	std::string		temp;

	*path = config._servConf.front()._root;
	if (it != config._servConf.front()._locations.end())
	{
		if (!config._servConf.front()._locations[it->first]._root.empty())
			*path = config._servConf.front()._locations[it->first]._root;
		if (indexSetup(config, it->first, path))
			return ;
	}
	temp = *path;
	for (size_t i = 0; i < config._servConf.front()._index.size(); i++)
	{
		*path = temp;
		path->append(config._servConf.front()._index[i]);
		ifs->open(*path);
		std::cout << RED << "Path found : " << *path << std::endl;
		if (*ifs)
			return ;
	}
	_errorCode = 404;
	*path = temp;
}

bool	GetMethod::indexSetup(ServerConfig& config, std::string key, std::string* path)
{
	std::string		temp;
	std::ifstream	ifs;

	if (config._servConf.front()._locations[key]._index.size() != 0)
	{
		temp = *path;
		for (size_t i = 0; i < config._servConf.front()._locations[key]._index.size(); i++)
		{
			*path = temp;
			path->append(config._servConf.front()._locations[key]._index[i]);
			std::cout << RED << "Path found : " << *path << std::endl;
			ifs.open(*path);
			if (ifs)
			{
				std::cout << "IT Works" << std::endl;
				return (true);
			}
			else
				std::cout << "Doesn't work" << std::endl;
		}
	}
	temp = *path;
	for (size_t i = 0; i < config._servConf.front()._index.size(); i++)
	{
		*path = temp;
		path->append(config._servConf.front()._index[i]);
		ifs.open(*path);
		if (ifs)
			return (true);
	}
	_errorCode = 404;
	*path = temp;
	return (false);
}

bool	GetMethod::errorCodeSetup(ServerConfig& config, std::string* path, HandleResponse* hand_resp)
{
	std::map<int, std::string>::iterator	it = config._servConf.front()._errorPages.find(_errorCode);

	if (it != config._servConf.front()._errorPages.end())
	{
		path->append(it->second);
		if (it->first == 404)
			hand_resp->set_status_line("HTTP/1.1", "404", "Not Found");
		else if (it->first == 401)
			hand_resp->set_status_line("HTTP/1.1", "401", "Unauthorized");
		else if (it->first == 403)
			hand_resp->set_status_line("HTTP/1.1", "401", "Forbidden Access");
		return (true);
	}
	else if (_errorCode)
	{
		hand_resp->set_status_line("HTTP/1.1", "404", "Not Found");
		path->append("/404.html");
		std::cout << RED << "On error, path : " << *path << std::endl;
		return (true);
	}
	else
		return (false);
}