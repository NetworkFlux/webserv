#include "../../include/Messages/Response.hpp"

/*	This contructor initialize all members for use.	*/
Response::Response(void) : _status_line(), _header(), _body(), _protocol(), _status_code(0),
	_status_message(), _content_length(0), _content_type()
{}

/*	This contructor creates the status line by taking its parameters (protocol, status code, status message)
	ans initialize the other member for later use.	*/
Response::Response(const std::string& protocol, size_t status_code, const std::string& status_message) :
	_status_line(), _header(), _body(), _protocol(protocol), _status_code(status_code),
	_status_message(status_message), _content_length(0), _content_type()
{
	update_status_line();
}

Response::~Response(void)
{}

Response&	Response::operator=(const Response& other)
{
	if (this == &other)
		return (*this);
	_status_line = other._status_line;
	_header = other._header;
	_body = other._body;
	_protocol = other._protocol;
	_status_code = other._status_code;
	_status_message = other._status_message;
	_content_length = other._content_length;
	_content_type = other._content_type;
	return (*this);
}

std::string		Response::get_status_line(void) const
{
	return (_status_line);
}

std::map<std::string, std::string>	Response::get_header(void) const
{
	return (_header);
}

std::vector<char>		Response::get_body(void) const
{
	return (_body);
}

std::string		Response::get_protocol(void) const
{
	return (_protocol);
}

size_t	Response::get_status_code(void) const
{
	return (_status_code);
}

std::string		Response::get_status_message(void) const
{
	return (_status_message);
}

void	Response::set_status_line(const std::string& protocol, size_t status_code, const std::string& status_message)
{
	_protocol = protocol;
	_status_code = status_code;
	_status_message = status_message;
	update_status_line();
}

void	Response::set_header(const std::map<std::string, std::string>& header)
{
	_header = header;
}

void	Response::set_body(const std::vector<char>& body)
{
	_body = body;
	set_content_length(_body.size());
}

void	Response::set_content_length(size_t content_length)
{
	_content_length = content_length;
	update_header();
}

void	Response::set_content_type(const std::string& content_type)
{
	_content_type = content_type;
	update_header();
}

void	Response::set_protocol(const std::string& protocol)
{
	_protocol = protocol;
	update_status_line();
}

void	Response::set_status_code(size_t status_code)
{
	_status_code = status_code;
	update_status_line();
}

void	Response::set_status_message(const std::string& status_message)
{
	_status_message = status_message;
	update_status_line();
}

void	Response::show_status_line(void) const
{
	std::cout << YELLOW << "Status Line: " << _status_line << NONE << std::endl;
}

void	Response::show_header(void) const
{
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); ++it)
		std::cout << YELLOW << it->first << ": " << it->second << NONE << std::endl;
}

void	Response::show_body(void) const
{
	for (std::vector<char>::const_iterator it = _body.begin(); it != _body.end(); ++it)
		std::cout << YELLOW << *it << NONE;
	std::cout << std::endl;
}

void	Response::show_data(void) const
{
	std::cout << YELLOW_B << "Response data:" << YELLOW << std::endl;
	std::cout << "Status line: " << _status_line << std::endl;
	show_header();
	show_body();
}

void	Response::build_response(const std::string& path)
{
	if (path.find("html") != std::string::npos)
		set_content_type("text/html");
	else if (path.find("jpeg") != std::string::npos)
		set_content_type("image/jpeg");
	else if (path.find("ico") != std::string::npos)
		set_content_type("image/x-icon");
	else if (path.find("png") != std::string::npos)
		set_content_type("image/png");
	else if (path.find("webp") != std::string::npos)
		set_content_type("image/webp");
	set_body(readBinaryFile(path));

}

std::string Response::give_response(void)
{
	// Start building the response
	std::string response = _status_line;
	// Add the header fields to the response
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); ++it)
	{
	response += it->first + ": " + it->second + "\r\n";
	}

	// Add a blank line after the header
	response += "\r\n";

	// Append the body to the response
	response.append(_body.begin(), _body.end());

	// Return the complete response
	return (response);
}

void	Response::update_status_line(void)
{
	_status_line = _protocol + " ";
	_status_line += std::to_string(_status_code) + " ";
	_status_line += _status_message + "\r\n";
}

void	Response::update_header(void)
{
	_header["Content-Length"] = std::to_string(_content_length);
	_header["Content-Type"] = _content_type;
}