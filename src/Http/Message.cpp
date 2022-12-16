#include "../../include/Http/Message.hpp"

Message::Message(void) 
{

}

Message::Message(Message const &x)
{
    *this = x;
}
Message &Message::operator=(Message const &x)
{
    _headers = x._headers;
    _body = x._body;
    return *this;
}

Message::~Message(void)
{
    
}

const std::map<std::string, std::string> &Message::get_headers() const
{
    return (_headers);
}

std::string &Message::get_header(std::string key) 
{
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return (_headers[key]);
}

const std::string &Message::get_body(void) const
{ 
    return (_body);
}

void Message::add_header(std::string key, const std::string &content)
{
    // std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    _headers.insert(std::pair<std::string, std::string>(key, content));

}

void Message::set_body(const std::string & body)
{
	_body = body;
}

