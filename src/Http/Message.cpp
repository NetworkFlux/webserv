#include "../../include/Http/Message.hpp"

Message::Message()
{}

Message::~Message()
{}

/*	Returns a specific header thanks to its key.	*/
std::string&	Message::get_header(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	return (_headers[key]);
}

/*	Returns all the headers.	*/
const std::map<std::string, std::string>&	Message::get_headers() const
{
	return (_headers);
}

/*	Returns the body of the message.	*/
const std::string&	Message::get_body() const
{
	return (_body);
}

/*	Sets the body of the message.	*/
void	Message::set_body(const std::string& body)
{
	_body = body;
}

/* Adds a header (map<str, str>) to all the headers already existing in the message.	*/
void	Message::add_header(std::string key, const std::string& content)
{
	_headers.insert(std::pair<std::string, std::string>(key, content));
}