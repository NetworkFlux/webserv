#include "../include/HandleHttp.hpp"

HandleHttp::HandleHttp(const std::string& request_line, ServerConfig* serverConfig, size_t serv_index) : _request(request_line),
	_response(), _location(), _final_path(), _failed_request(false)
{
	if (_request.get_status_line().empty())
		_failed_request = true;
	int pos;
	_req_path = _request.get_path();
	if ((pos = _req_path.find('?')) != -1)
		_req_path = _request.get_path().substr(0, pos);
	_request.set_header(request_line);
	_config = serverConfig->_servConf[static_cast<unsigned int>(serv_index)];
	find_location();
}

HandleHttp::~HandleHttp()
{}

void	HandleHttp::do_work(void)
{
	SimpleConfig	loc_config = _config._locations[_location];

	if (!check_protocol())
	{
		_response.set_status_line("HTTP/1.1", 505 ,"HTTP Version Not Supported");
		return (build_response(loc_config));
	}
	// Before doing anything, check the root and add it to _final_path
	if (check_root(loc_config._root))
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return (build_response(loc_config));
	}
	// Check if the method is allowed at that location
	if (check_method_allowed(loc_config._methods, _config._methods, _request.get_method()))
	{
		_response.set_status_line("HTTP/1.1", 405 ,"Method Not Allowed");
		return (build_response(loc_config));
	}
	// Verify if the request body is not too big
	if (check_max_body(loc_config._maxBody, _request.get_body()))
	{
		_response.set_status_line("HTTP/1.1", 413 ,"Payload Too Large");
		return (build_response(loc_config));
	}
	// Check for redirections
	if (!loc_config._redirect.empty())
	{
		redirection(loc_config);
		return ;
	}
	if (_request.get_method() == "DELETE")
	{
		if (!delete_file())
			_response.set_status_line("HTTP/1.1", 202 ,"Accepted");
		else
			std::cout << "final path: " << _final_path << std::endl;
		return (build_response(loc_config));
	}
	if (_req_path == "/download.html")
		return (build_directory_listing_down());
	// Check the asked path IS the index file (needed because index file may not be in the location folder, so wont be found otherwise)
	if (is_index_file(loc_config, _config._index))
		return ;
	// Check the asked path : if it's a directory, check if there is an index file
	DIR *tdir = opendir((_final_path + _req_path).c_str());
	if ((tdir != NULL))
	{
		closedir (tdir);
		_final_path += _req_path;
		if (!check_index(loc_config, _config._index))
		{
			_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
			return (build_response(loc_config));
		}
		return ;
	}
	// Verify the file extension : if it's a cgi, execute it
	if (check_cgi_extension(_req_path))
	{
		if (!execute_cgi())
			return (build_response(loc_config));
		return;
	}
	std::string tmp_final_path = _final_path + _req_path;
	std::cout << RED << "\tTemp final path: " << tmp_final_path << NONE << std::endl;
	// If we get here it means the client asked for a file
	if (file_exists(tmp_final_path))
	{
		_final_path = tmp_final_path;
		_response.set_status_line("HTTP/1.1", 200 ,"OK");
		return (build_response(loc_config));
	}
	else
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return (build_response(loc_config));
	}
	return ;
}

Response&	HandleHttp::get_response(void)
{
	return (_response);
}

void	HandleHttp::build_response(SimpleConfig& loc_config)
{
	if (_response.get_status_code() > 200 && _response.get_status_code() < 600)
	{
		std::map<int, std::string>::iterator	it = loc_config._errorPages.find(_response.get_status_code());
		if (it == loc_config._errorPages.end())
		{
			it = _config._errorPages.find(_response.get_status_code());
			if (it == _config._errorPages.end())
			{
				// _response.set_status_line("HTTP/1.1", 404 ,"Not Found");
				_final_path += _config._errorPages[404];
			}
			else
				_final_path += _config._errorPages[_response.get_status_code()];
		}
		else
			_final_path += loc_config._errorPages[_response.get_status_code()];
		// _response.set_content_type("text/html");
	}
	if (_final_path.find(".html") != std::string::npos)
		_response.set_content_type("text/html");
	else if (_final_path.find(".jpeg") != std::string::npos)
		_response.set_content_type("image/jpeg");
	else if (_final_path.find(".ico") != std::string::npos)
		_response.set_content_type("image/x-icon");
	else if (_final_path.find(".png") != std::string::npos)
		_response.set_content_type("image/png");
	else if (_final_path.find(".webp") != std::string::npos)
		_response.set_content_type("image/webp");

	std::cout << RED << "\tFinal path: " << _final_path << NONE << std::endl;
	_response.set_body(readBinaryFile(_final_path));
}

bool	HandleHttp::execute_cgi(void)
{
	std::cout << RED << "\tCGI" << NONE << std::endl;
	CGIServer cgi(_config._root, _request.get_path(), _request.get_method());
	std::string response;
	if (cgi.run_program(_request.get_header(), _request.get_body(), &response) == -1)
	{
		_response.set_status_line("HTTP/1.1", 502 ,"Bad Gateway");
		_final_path = "www";
		return (false);
	}
	std::string contentType;
	std::size_t pos = response.find("Content-Type:");
	if (pos != std::string::npos) {
        std::size_t newLinePos = response.find("\n", pos);
        contentType = response.substr(pos + 13, newLinePos - pos - 13);
        response.erase(pos, newLinePos - pos);
    }
	_response.set_content_type(contentType);	
	_response.set_body(str_to_vector(response));
	_response.set_status_line("HTTP/1.1", 200 ,"OK");
	return (true);
}

bool	HandleHttp::delete_file(void)
{
	std::string	fileToDel = _request.get_path().substr(_request.get_path().find_last_of('=') + 1, _request.get_path().size());
	std::string	fullPath = "www" + fileToDel;
	std::cout << "Deleting file: " << fullPath << std::endl;
	if (file_exists(fullPath))
	{
		if (remove(fullPath.c_str()) == 0)
		{
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
			_final_path += "/del/deleted.html";
			return (true);
		}
	}
	return (false);
}

void	HandleHttp::redirection(SimpleConfig& loc_config)
{
	_response.set_status_line("HTTP/1.1", 301 ,"Moved Permanently");
	_response.add_header("Location", loc_config._redirect);
	_response.add_header("Content-Length", "0");
}

void	HandleHttp::show_request()
{
	_request.show_data();
	std::cout << RED_B << "Processed" << NONE << std::endl;
	std::cout << RED << "\tLocation: " << _location << NONE << std::endl;
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

bool	HandleHttp::check_protocol(void)
{
	if (_request.get_protocol() != "HTTP/1.1")
		return false;
	return true;
}

bool	HandleHttp::check_method_allowed(const std::vector<std::string>& loc_methods, const std::vector<std::string>& conf_methods, const std::string& asked_method)
{
	std::vector<std::string>::const_iterator	it1, it2;
	it1 = std::find(loc_methods.begin(), loc_methods.end(), asked_method);
	it2 = std::find(conf_methods.begin(), conf_methods.end(), asked_method);
	if (it1 == loc_methods.end() && it2 == conf_methods.end())
	return (true);
	std::cout << RED << "\tMethod allowed" << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_root(const std::string& root)
{
	if (root.empty() && _config._root.empty())
	{
		_final_path = "www";
		return (true);
	}
	if (!root.empty())
	{
		_final_path = root;
		_req_path.erase(_req_path.find(_location), _location.size());
		if (_req_path.empty())
			_req_path = "/";
	}
	else
		_final_path = _config._root;
	std::cout << RED << "\tRoot: " << _final_path << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_index(SimpleConfig& loc, const std::vector<std::string>& conf_index) // Must indeed check all the index // ERROR 403 WHEN NONE ???
{
	std::vector<std::string> loc_index = loc._index;
	for (std::vector<std::string>::const_iterator it = loc_index.begin(); it != loc_index.end(); ++it)
	{
		if (file_exists(_final_path + *it))
		{
			_final_path += *it;
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
			build_response(loc);
			return (true);
		}
	}
	for (std::vector<std::string>::const_iterator it = conf_index.begin(); it != conf_index.end(); ++it)
	{
		if (file_exists(_final_path + *it))
		{
			_final_path += *it;
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
			build_response(loc);
			return (true);
		}
	}
	if (loc._autoIndex)
	{
		std::cout << RED << "\tAutoIndex : ON" << NONE << std::endl;
		build_directory_listing();
		return (true);
	}
	return (false);
}

bool HandleHttp::is_index_file(SimpleConfig& loc, const std::vector<std::string>& conf_index)
{
	std::vector<std::string> loc_index = loc._index;
	for (std::vector<std::string>::const_iterator it = loc_index.begin(); it != loc_index.end(); ++it)
	{
		if (_req_path.substr( _req_path.find_last_of('/'), (_req_path.size() - _req_path.find_last_of('/'))) == *it /* file_exists(_final_path + *it) */)
		{
			_final_path += *it;
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
			build_response(loc);
			return (true);
		}
	}
	for (std::vector<std::string>::const_iterator it = conf_index.begin(); it != conf_index.end(); ++it)
	{
		if (_req_path.substr( _req_path.find_last_of('/'), (_req_path.size() - _req_path.find_last_of('/'))) == *it /* file_exists(_final_path + *it) */)
		{
			_final_path += *it;
			_response.set_status_line("HTTP/1.1", 200 ,"OK");
			build_response(loc);
			return (true);
		}
	}
	return (false);
}

bool	HandleHttp::check_cgi_extension(const std::string& path)
{
	std::string ext = path.substr(path.find_last_of(".") + 1);
	std::vector<std::string>::iterator	it = std::find(_config._cgiExt.begin(), _config._cgiExt.end(), ext);
	if (it == _config._cgiExt.end())
		return (false);
	std::cout << RED << "\tExtension: " << ext << NONE << std::endl;
	_final_path += path;
	return (true);
}

bool	HandleHttp::check_max_body(long max_body, const std::string& body)
{
	if (max_body == -1 && _config._maxBody == -1)
		return (false);
	else
	{
		if (max_body == -1)
		{
			if (body.size() > (size_t)_config._maxBody)
				return (true);
		}
		else
		{
			if (body.size() > (size_t)max_body)
				return (true);
		}
	}
	return (false);
}

void	HandleHttp::build_directory_listing()
{
	struct dirent *dir;
    DIR *pDir;

	pDir = opendir(_final_path.c_str());
	if (pDir == NULL) {
		printf ("Cannot open directory '%s' \n", _final_path.c_str());
	}
	std::string response = "<!DOCTYPE html> <html> <h1>Index of " + _req_path + "</h1>";
	response += "<table><thead><tr><th role=\"button\">Name</th></tr></thead><tbody>";
	while ((dir = readdir(pDir)) != NULL) {
        response += "<tr>";
		response += "<td><a href=\"http://" + _config._serverNames[0] + ":" + _config._listen.find("127.0.0.1")->second + _req_path;
		response += dir->d_name;
		response += "\"> ";
		response += dir->d_name;
		response += "</a></td></tr>\n";
    }
    closedir (pDir);
	response += "</tbody></table></html>";
	_response.set_body(str_to_vector(response));
	_response.set_status_line("HTTP/1.1", 200 ,"OK");
}

void	HandleHttp::build_directory_listing_down()
{
	struct dirent *dir;
    DIR *pDir;

	_final_path = "www/uploads/";
	pDir = opendir(_final_path.c_str());
	if (pDir == NULL) {
		printf ("Cannot open directory '%s' \n", _final_path.c_str());
	}
	std::string response = "<!DOCTYPE html> <html> <h1>Downloads</h1>";
	response += "<table><thead><tr><th role=\"button\">Name</th></tr></thead><tbody>";
	int i = 0;
	while ((dir = readdir(pDir)) != NULL) {
		if (i > 1)
		{
        	response += "<tr>";
			response += "<td><a href=\"/uploads/";
			response += dir->d_name;
			response += "\"download> ";
			response += dir->d_name;
			response += "</a></td></tr>\n";
		}
		i++;
    }
    closedir (pDir);
	response += "</tbody></table></html>";
	_response.set_body(str_to_vector(response));
	_response.set_status_line("HTTP/1.1", 200 ,"OK");
}

bool	HandleHttp::client_close()
{
	std::map<std::string, std::string> header = _request.get_header();
	if (header["Connection"] == "keep-alive\r")
		return false;
	else if (header["Connection"] == "close\r")
		return true;
	return false;
}