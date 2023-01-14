#include "../include/HandleHttp.hpp"

HandleHttp::HandleHttp(const std::string& request_line, ServerConfig* serverConfig, size_t serv_index) : _request(request_line),
	_response(), _location(), _directory_listing(false), _final_path(), _failed_request(false)
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

	if (!check_root(loc_config._root))
	{
		if (!check_method_allowed(loc_config._methods, _config._methods, _request.get_method()))
		{
			if (check_index(loc_config._index, _config._index))
			{
				if (_directory_listing)
				{
					if (_config._locations[_location]._autoIndex == true)
						return (build_directory_listing());
					else
						_response.set_status_line("HTTP/1.1", 403 ,"Forbidden Access");
				}
				return (build_response(loc_config));
			}
		}
		else
			return (build_response(loc_config));
	}
	if (_location == "/upload")
		upload_file();
	if (_location == "/delete")
		delete_file();
	if (_location == "/redirect")
		redirection(loc_config);
	if (_location == "/cgi" && execute_cgi())
		return ;
	build_response(loc_config);
}

Response&	HandleHttp::get_response(void)
{
	return (_response);
}

void	HandleHttp::build_response(SimpleConfig& loc_config)
{
	if (_failed_request)
		_response.set_status_line("HTTP/1.1", 400 ,"Bad Request");
	if (_response.get_status_code() > 200 && _response.get_status_code() < 500)
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

	std::cout << RED << "\tFinal Path: " << _final_path << NONE << std::endl << std::endl << std::endl;
	_response.set_body(readBinaryFile(_final_path));
}

bool	HandleHttp::execute_cgi(void)
{
	std::cout << RED << "\tCGI" << NONE << std::endl;
	CGIServer cgi("www", _request.get_path(), _request.get_method());
	std::string response;
	int error = cgi.run_program(_request.get_header(), _request.get_body(), &response);
	if (error == -1)
	{
		_response.set_status_line("HTTP/1.1", 502 ,"Bad Gateway");
		return (false);
	}
	_response.set_body(str_to_vector(response));
	_response.set_status_line("HTTP/1.1", 200 ,"OK");
	return (true);
}

void	HandleHttp::upload_file(void)
{
	std::cout << RED << "\tUpload file" << NONE << std::endl;
	std::string body = _request.get_body();
	std::vector<std::string> data = splitLine(body, "&");

	std::string file_name = data[0].substr(data[0].find("=") + 1);
	std::string file_content = data[1].substr(data[1].find("=") + 1);

	// Create file and write content
	std::ofstream file;
	std::cout << BLUE_B << "\tFile name: " << ("www" + _location + "/" + file_name) << NONE << std::endl;
	file.open("www" + _location + "/" + file_name);
	if (!file)
	{
		std::cout << RED << "Error: File not created" << NONE << std::endl;
		return ;
	}
	file << file_content;
}

void	HandleHttp::delete_file()
{
	std::cout << RED << "\tDelete file" << NONE << std::endl;
	std::string body = _request.get_body();
	std::string up_files = "www";
	std::vector<std::string> data = splitLine(body, "&");

	std::string file_name = data[0].substr(data[0].find("=") + 1);
	up_files += "/upload/";
	up_files += file_name;

	std::cout << BLUE_B << "\tFile name: " << up_files << NONE << std::endl;

	// Delete file
	if (remove(up_files.c_str()) != 0)
	{
		std::cout << RED << "Error: File not deleted" << NONE << std::endl;
		return ;
	}
}

void	HandleHttp::redirection(SimpleConfig& loc_config)
{
	if (loc_config._redirect.empty())
	{
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return ;
	}
	_response.set_status_line("HTTP/1.1", 301 ,"Moved Permanently");
	_response.add_header("Location", loc_config._redirect);
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
	std::cout << RED << "\tMethod allowed" << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_root(const std::string& root)
{
	if (root.empty() && _config._root.empty())
	{
		_final_path = "www";
		_response.set_status_line("HTTP/1.1", 404 ,"Not Found");
		return (true);
	}
	if (!root.empty())
		_final_path = root;
	else
		_final_path = _config._root;
	std::cout << RED << "\tRoot: " << _final_path << NONE << std::endl;
	return (false);
}

bool	HandleHttp::check_index(const std::vector<std::string>& loc_index, const std::vector<std::string>& conf_index) // Must indeed check all the index // ERROR 403 WHEN NONE ???
{
	if (_req_path == _location && _location != "/")
		_req_path += "/";
	if ((_req_path.size() > _location.size()) && (_req_path[_req_path.size()-1] != '/'))
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
		std::cout << RED << "\tFile: " << _final_path << NONE << std::endl;
		return (false);
	}
	else
	{
		_directory_listing = true;
		return (true);
	}	
}

void	HandleHttp::build_directory_listing()
{
	struct dirent *dir;
    DIR *pDir;
	std::string full_path = _final_path + _req_path;

	pDir = opendir(full_path.c_str());
	if (pDir == NULL) {
		printf ("Cannot open directory '%s' \n", full_path.c_str());
	}
	std::string response = "<!DOCTYPE html> <html> <h1>Index of " + _req_path + "</h1>";
	response += "<table><thead><tr><th role=\"button\">Name</th></tr></thead><tbody>";
	while ((dir = readdir(pDir)) != NULL) {
        response += "<tr>";
		response += "<td><a href=\"http://" + _config._serverNames[0] + ":" + _config._listen[0] + _req_path;
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


bool	HandleHttp::client_close()
{
	std::map<std::string, std::string> header = _request.get_header();
	if (header["Connection"] == "keep-alive")
		return false;
	return true;
}