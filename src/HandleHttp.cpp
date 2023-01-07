#include "../include/HandleHttp.hpp"

HandleHttp::HandleHttp(const std::string& request_line, ServerConfig* serverConfig, size_t serv_index) : _request(request_line),
	_response(), _location(), _final_path()
{
	_req_path = _request.get_path();
	_config = serverConfig->_servConf[static_cast<unsigned int>(serv_index)];
	find_location();
}

HandleHttp::~HandleHttp()
{}

void	HandleHttp::do_work(void)
{
	SimpleConfig	loc_config = _config._locations[_location];

	if (!check_root(loc_config._root))
	{
		if (!check_method_allowed(loc_config._methods, _config._methods, _request.get_method()))
			check_index(loc_config._index, _config._index);
	}
	build_response(loc_config);
}

Response&	HandleHttp::get_response(void)
{
	return (_response);
}

void	HandleHttp::build_response(SimpleConfig& loc_config)
{
	if (_response.get_status_code() > 200)
	{
		std::map<int, std::string>::iterator	it = loc_config._errorPages.find(_response.get_status_code());
		if (it == loc_config._errorPages.end())
		{
			it = _config._errorPages.find(_response.get_status_code());
			if (it == _config._errorPages.end())
			{
				_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
				_final_path += _config._errorPages[404];
			}
			else
				_final_path += _config._errorPages[_response.get_status_code()];
		}
		else
			_final_path += loc_config._errorPages[_response.get_status_code()];
	}
	else // 200
	{
		_response.set_status_line("HTTP/1.1", 200 ,"OK");
	}
	if (_final_path.find("html") != std::string::npos)
		_response.set_content_type("text/html");
	else if (_final_path.find("jpeg") != std::string::npos)
		_response.set_content_type("image/jpeg");
	else if (_final_path.find("ico") != std::string::npos)
		_response.set_content_type("image/x-icon");
	else if (_final_path.find("png") != std::string::npos)
		_response.set_content_type("image/png");
	else if (_final_path.find("webp") != std::string::npos)
		_response.set_content_type("image/webp");

	std::cout << RED << "Final Path: " << _final_path << NONE << std::endl;
	_response.set_body(readBinaryFile(_final_path));
}

void	HandleHttp::show_request()
{
	_request.show_data();
	std::cout << RED << "Location: " << _location << NONE << std::endl;
}

void	HandleHttp::show_response()
{
	_response.show_data();
}

void	HandleHttp::find_location(void)
{
	size_t		holder = 0;
	std::map<std::string, SimpleConfig>::iterator	it = _config._locations.begin();

	for (; it != _config._locations.end(); ++it)
	{
		if (find_in_str(it->first, _req_path) > holder)
		{
			holder = find_in_str(it->first, _req_path);
			_location = it->first;
		}
	}
}

bool	HandleHttp::check_method_allowed(const std::vector<std::string>& loc_methods, const std::vector<std::string>& conf_methods, const std::string& asked_method)
{
	std::vector<std::string>::const_iterator	it1, it2;

	it1 = std::find(loc_methods.begin(), loc_methods.end(), asked_method);
	it2 = std::find(conf_methods.begin(), conf_methods.end(), asked_method);
	if (it1 == loc_methods.end() && it2 == conf_methods.end())
	{
		_response.set_status_line("HTTP/1.1", 405 ,"Method Not Allowed");
		return (true);
	}
	std::cout << RED << "Method allowed" << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_root(const std::string& root)
{
	if (root.empty() && _config._root.empty())
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return (true);
	}
	if (!root.empty())
		_final_path = root;
	else
		_final_path = _config._root;
	std::cout << RED << "Root: " << _final_path << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_index(const std::vector<std::string>& loc_index, const std::vector<std::string>& conf_index) // Must indeed check all the index // ERROR 403 WHEN NONE ???
{
	if (_req_path.size() > _location.size())
	{
		if (file_exists(_final_path + _req_path))
		{
			_final_path += _req_path;
			return (false);
		}
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return (true);
	}
	for (std::vector<std::string>::const_iterator it = loc_index.begin(); it != loc_index.end(); ++it)
	{
		if (file_exists(_final_path + *it))
		{
			_final_path += *it;
			return (true);
		}
	}
	for (std::vector<std::string>::const_iterator it = conf_index.begin(); it != conf_index.end(); ++it)
	{
		if (file_exists(_final_path + *it))
		{
			_final_path += *it;
			return (true);
		}
	}
	if (file_exists(_final_path + _req_path))
	{
		_final_path += _req_path;
		_response.set_status_line("HTTP/1.1", 200 ,"OK");
		std::cout << RED << "File: " << _final_path << NONE << std::endl;
		return (false);
	}
	_response.set_status_line("HTTP/1.1", 403 ,"Forbidden Access");
	return (true);
}