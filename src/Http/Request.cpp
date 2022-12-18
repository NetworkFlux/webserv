#include "../include/Http/Request.hpp"

/*	Initialize the member to empty strings.	*/
Request::Request()
{
	_method = "";
	_path = "";
	_protocol = "";
}

Request::~Request()
{}

/*	Return the requested method.	*/
const std::string&	Request::get_method(void)
{
	return (_method);
}

/*	Return the requested path.	*/
const std::string&	Request::get_path(void)
{
	return (_path);
}

/*	Return the requested protocol version.	*/
const std::string&	Request::get_protocol(void)
{
	return (_protocol);
}

/*	Set the requested method.	*/
void	Request::set_method(const std::string& method)
{
	_method = method;
}

/*	Set the requested path.	*/
void	Request::set_path(const std::string& uri)
{
	_path = uri;
}

/*	Set the requested protocol version.	*/
void	Request::set_protocol(const std::string& version)
{
	_protocol = version;
}