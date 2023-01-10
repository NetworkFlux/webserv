#include "../../include/Messages/Request.hpp"

Request::Request() : _status_line(), _method(), _path(), _protocol()
{}

Request::Request(const std::string& status_line) : _status_line(get_first_line(status_line))
{
	// Parse the request line
	std::vector<std::string> request_line_vector = splitLine(get_first_line(status_line), " ");
	// if (request_line_vector.size() != 3)
	// 	throw std::runtime_error("Request line is not valid");
	_method = request_line_vector[0];
	_path = request_line_vector[1];
	_protocol = request_line_vector[2];
	_body = getHttpRequestBody(status_line);
}

Request::~Request()
{}

Request&	Request::operator=(const Request& other)
{
	if (this == &other)
		return (*this);
	_status_line = other._status_line;
	_method = other._method;
	_path = other._path;
	_protocol = other._protocol;
	return (*this);
}

const std::string&	Request::get_status_line(void) const
{
	return (_status_line);
}

const std::string&	Request::get_method(void) const
{
	return (_method);
}

const std::string&	Request::get_path(void) const
{
	return (_path);
}

const std::string&	Request::get_protocol(void) const
{
	return (_protocol);
}

const std::map<std::string, std::string>&	Request::get_header(void) const
{
	return (_header);
}

const std::string&	Request::get_body(void) const
{
	return (_body);
}

void	Request::set_status_line(const std::string& status_line)
{
	_status_line = status_line;
}

void	Request::set_method(const std::string& method)
{
	_method = method;
}

void	Request::set_path(const std::string& path)
{
	_path = path;
}

void	Request::set_protocol(const std::string& protocol)
{
	_protocol = protocol;
}

void	Request::set_header(const std::string& request)
{
	std::stringstream	ss(request);
	std::string			token;
	bool				first = false;
	while (getline(ss, token, '\n'))
	{
		if (token == "\r")
			break ;
		if (first)
		{
			std::istringstream token_iss(token);
			std::string key, value;
			std::getline(token_iss, key, ':');
			std::getline(token_iss, value, ':');
			_header[key] = value;
		}
		first = true;
	}
  	std::string remaining_string((std::istreambuf_iterator<char>(ss)), std::istreambuf_iterator<char>());
	set_body(remaining_string);
}

void	Request::set_body(const std::string& str)
{
	_body = str;
}


void	Request::show_data(void) const
{
	std::cout << GREEN_B << "Request data:" << GREEN << std::endl;
	std::cout << "\tRequest line: " << _status_line << std::endl;
	std::cout << "\tMethod: " << _method << std::endl;
	std::cout << "\tPath: " << _path << std::endl;
	std::cout << "\tProtocol: " << _protocol << std::endl << std::endl;
	std::cout << "\tBody:" << std::endl << "\t" << _body << NONE << std::endl << std::endl;
}