#include "../../include/Http/Response.hpp"

/*	Initialize the member to empty strings.	*/
Response::Response()
{
	_protocol = "";
	_status = "";
	_reason = "";
}

Response::~Response()
{}

/*	Return the requested protocol version.	*/
const std::string&	Response::get_protocol(void)
{
	return (_protocol);
}

/*	Return the requested status.	*/
const std::string&	Response::get_status(void)
{
	return (_status);
}

/*	Return the requested reason.	*/
const std::string&	Response::get_reason(void)
{
	return (_reason);
}

/*	Set the requested protocol version.	*/
void	Response::set_protocol(const std::string& version)
{
	_protocol = version;
}

/*	Set the requested status.	*/
void	Response::set_status(const std::string& status)
{
	_status = status;
}

/*	Set the requested reason.	*/
void	Response::set_reason(const std::string& reason)
{
	_reason = reason;
}