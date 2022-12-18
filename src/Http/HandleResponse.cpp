#include "../../include/Http/HandleResponse.hpp"

/*	Initialize the member variables.	*/
HandleResponse::HandleResponse()
{
	_response = new Response();
	_string_response = "";
}

HandleResponse::~HandleResponse()
{
	delete _response;
}

/*	Returns the required Response member.	*/
Response&	HandleResponse::get_response(void)
{
	return (*_response);
}

/*	Returns the string that will be used as a response.	*/
const std::string&	HandleResponse::get_string_response(void)
{
	return (_string_response);
}

/*	This function will set the protocol, the status and the reason into the
	Response member.	*/
void	HandleResponse::set_status_line(std::string protocol, std::string status, std::string reason)
{
	_response->set_protocol(protocol);
	_response->set_status(status);
	_response->set_reason(reason);
}

/*	Adds the body string into the message.	*/
void	HandleResponse::set_body(std::string body)
{
	parse_body(body);
}

/*	This function will build a good string that will be accepted as a server
	response.	*/
void	HandleResponse::make_string_response(void)
{
	std::map<std::string, std::string>				header;
	std::map<std::string, std::string>::iterator	it;

	_string_response.append(_response->get_protocol());
	_string_response.append(" ");
	_string_response.append(_response->get_status());
	_string_response.append(" ");
	_string_response.append(_response->get_reason());
	_string_response.append("\n");
	header = get_message_header();
	for (it = header.begin(); it != header.end(); it++)
	{
		_string_response.append(it->first); + ": " + it->second + "\n";
		_string_response.append(": ");
		_string_response.append(it->second);
		_string_response.append("\n");
	}
	_string_response.append("\n");
	_string_response.append(get_message_body());
}

/*	This function builds a functioning header that must be used as a part of the
	response.	*/
void	HandleResponse::make_header(const std::string& body)
{
	std::string	token;

	token.append("Content-Length: ");
	token.append(std::to_string(body.length()));
	parse_headers(token);
	token.clear();
	token.append("Content-Type: text/html");
	parse_headers(token);
}