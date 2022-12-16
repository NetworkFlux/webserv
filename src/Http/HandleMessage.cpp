#include "../../include/Http/HandleMessage.hpp"

HandleMessage::HandleMessage(void)
{
    _message = new Message();
}

HandleMessage::HandleMessage(HandleMessage const & x)
{
    *this = x;
}

HandleMessage &HandleMessage::operator=(HandleMessage const &x)
{
    if (this != &x)
		_message = x._message;
	return *this;
}

HandleMessage::~HandleMessage(void)
{
    delete _message;
}

Message *HandleMessage::get_message() const
{
    return _message;
}

void HandleMessage::set_message(Message *x)
{
    if (!x)
        return ;
    _message = x;
}

void HandleMessage::parse_headers(std::string line)
{
    std::string key;
    std::string content;
    size_t len = line.find(":");

    key = line.substr(0, len);
	content = line.substr(len + 2, line.length() - len - 2);

    _message->add_header(key, content);
}

void HandleMessage::parse_body(std::string body)
{
    _message->set_body(body);
}

const std::map<std::string, std::string> &HandleMessage::get_message_header(void) const
{
    return _message->get_headers();
}

const std::string &HandleMessage::get_message_body(void) const
{
    return _message->get_body();
}



