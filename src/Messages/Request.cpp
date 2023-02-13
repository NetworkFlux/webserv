#include "../../include/Messages/Request.hpp"

Request::Request() : _status_line(), _method(), _path(), _protocol()
{}

Request::Request(const std::string& request_string)
{
	_status_line = get_first_line(request_string);
	std::vector<std::string> request_line_vector = splitLine(get_first_line(request_string), " ");
	if (request_line_vector.size() != 3)
		return ;
	_method = request_line_vector[0];
	_path = request_line_vector[1];
	_protocol = request_line_vector[2];
	_body = getHttpRequestBody(request_string);
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

void Request::set_header(const std::string& request)
{
    std::stringstream ss(request);
    std::string token;

    while (std::getline(ss, token)) // use default delimiter '\n'
    {
        if (token == "\r")
            break;

        // find the first colon in the token
        size_t colon_pos = token.find(':');
        if (colon_pos != std::string::npos)
        {
            // extract the key and value
            std::string key = token.substr(0, colon_pos);
            std::string value = token.substr(colon_pos + 1);
            // remove any leading or trailing whitespaces
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            _header[key] = value;
        }
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
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); it++)
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "\tBody:" << std::endl << "\t" << _body << NONE << std::endl << std::endl;
}
