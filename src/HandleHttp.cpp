#include "../include/HandleHttp.hpp"

HandleHttp::HandleHttp(const std::string& request_line, ServerConfig* serverConfig) : _request(request_line),
	_response(), _final_path()
{
	_serverConfig = serverConfig;
}

HandleHttp::~HandleHttp()
{}

Response&	HandleHttp::get_response(void)
{
	return (_response);
}

void	HandleHttp::do_work(void)
{
	SimpleConfig	config = _serverConfig->_servConf.front(); // Needs to be changed (only looking for the first server)
	std::map<std::string, SimpleConfig>::iterator	it;

	// This implementation will only work for GET requests
	if ((it = config._locations.find(_request.get_path())) != config._locations.end()) // Check if the path is in the config file
	{
		if (it->first == "/")	// Client is asking for the root
		{
			// Check if the method is allowed on / and then in root of config if not found
			// if not found build a 405 response
			if (check_method_allowed(it->second._methods, _request.get_method()))
			{
				_response.build_response(_final_path);
				return ;
			}
			// Check if the location has a root, if not check the root of the config
			// if not found build a 404 response
			if (check_root(it->second._root))
			{
				_response.build_response(_final_path);
				return ;
			}

			// Check if the location has a index, if not check the root of the config
			// if not found build a 404 response
			if (check_index(it->second._index))
			{
				_response.build_response(_final_path);
				return ;
			}

			// Check if the location has a autoindex, if not check the root of the config
			// if not found build a 404 response

			// Check if the location has a max_body_size, if not check the root of the config
			// if not found build a ? resopnse

			// All this functions will need to setup the response to be built

			// If we get here it means we encounterd no errors and we can setup a
			// valid response with the data given.
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
		}
		else
		{
			// I don't know if it has to behave differently if the path is not /
		}
	}
	else // If the path is not in the config file
	{
		// We should arrive here if the client asks for a path that is not in the config file
		_final_path = "www" + _request.get_path();
		if (file_exists(_final_path))
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
		else
		{
			_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
			_final_path = "www/404.html";
		}
	}
	_response.build_response(_final_path);
	return ;
}

bool	HandleHttp::check_method_allowed(const std::vector<std::string>& methods, const std::string& asked_method)
{
	std::vector<std::string>::const_iterator	it1, it2;

	it1 = std::find(methods.begin(), methods.end(), asked_method);
	it2 = std::find(_serverConfig->_servConf.front()._methods.begin(), _serverConfig->_servConf.front()._methods.end(), asked_method);
	if (it1 == methods.end() && it2 == _serverConfig->_servConf.front()._methods.end())
	{
		_response.set_status_line("HTTP/1.1", 405 ,"Method Not Allowed");
		_final_path = "www/405.html";
		return (true);
	}
	std::cout << "Method allowed" << std::endl;
	return (false);
}

bool	HandleHttp::check_root(const std::string& root)
{
	if (root.empty() && _serverConfig->_servConf.front()._root.empty())
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		_final_path = "www/404.html";
		return (true);
	}
	else
	{
		if (!root.empty())
			_final_path = root;
		else
			_final_path = _serverConfig->_servConf.front()._root;
	}
	return (false);
}

bool	HandleHttp::check_index(const std::vector<std::string>& index)
{
	//
	if (index.empty() && _serverConfig->_servConf.front()._index.empty())
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		_final_path = "www/404.html";
		return (true);
	}
	else
	{
		if (!index.empty())
			_final_path += index.front();
		else
			_final_path += _serverConfig->_servConf.front()._index.front();
	}
	return (false);
}

void	HandleHttp::show_request(void)
{
	_request.show_data();
}

void	HandleHttp::show_response(void)
{
	_response.show_data();
}

void	HandleHttp::show_response_status_line(void)
{
	_response.show_status_line();
}
