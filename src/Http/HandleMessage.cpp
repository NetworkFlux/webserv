#include "../../include/Http/HandleMessage.hpp"

HandleMessage::HandleMessage()
{
	_message = new Message();
}

HandleMessage::~HandleMessage()
{
	delete _message;
}

/* Returns all the header in the Message member.	*/
const std::map<std::string, std::string>&	HandleMessage::get_message_header(void) const
{
	return (_message->get_headers());
}

/*	Returns the entire body located in the Message member.	*/
const std::string&	HandleMessage::get_message_body(void) const
{
	return (_message->get_body());
}

/*	Returns a pointer to the actual Message member.	*/
Message*	HandleMessage::get_message(void) const
{
	return (_message);
}

/*	Sets a new message to hold by the Message member.	*/
void	HandleMessage::set_message(Message* x)
{
	if (x)
		_message = x;
}

/*	This function will parse the line string into a map<str, str> and adds this pair
	into the headers of the Message member.	*/
void	HandleMessage::parse_headers(std::string line)
{
	std::string	key;
	std::string	content;
	size_t		len = line.find(":");

	key = line.substr(0, len);
	content = line.substr(len + 2, line.length() - len - 2);
	_message->add_header(key, content);
}

/*	This function will parse the body string (it actually doesn't need to parse)
	and adds this new body into the Message member.	*/
void	HandleMessage::parse_body(std::string body)
{
	_message->set_body(body);
}
