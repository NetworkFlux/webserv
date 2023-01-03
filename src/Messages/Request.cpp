#include "../../include/Messages/Request.hpp"

Request::Request() : _status_line(), _method(), _path(), _protocol()
{}

Request::Request(const std::string& status_line) : _status_line(status_line)
{
	// Parse the request line
	std::vector<std::string> request_line_vector = splitLine(status_line, " ");
	if (request_line_vector.size() != 3)
		throw std::runtime_error("Request line is not valid");
	_method = request_line_vector[0];
	_path = request_line_vector[1];
	_protocol = request_line_vector[2];
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

void	Request::show_data(void) const
{
	std::cout << GREEN_B << "Request data:" << GREEN << std::endl;
	std::cout << "\tRequest line: " << _status_line << std::endl;
	std::cout << "\tMethod: " << _method << std::endl;
	std::cout << "\tPath: " << _path << std::endl;
	std::cout << "\tProtocol: " << _protocol << NONE << std::endl << std::endl;
}