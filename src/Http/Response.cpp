#include "../../include/Http/Response.hpp"

Response::Response() 
{
    _protocol = "";
    _status = "";
    _reason = "";
}

Response::Response(Response const &x)
{
    *this = x;
}

Response::~Response()
{

}

std::string const &Response::get_protocol(void)
{
    return _protocol;
}

std::string const & Response::get_status(void)
{
    return _status;
}

std::string const &Response::get_reason(void)
{
    return _reason;
}

void Response::set_protocol(std::string const &protocol)
{
    _protocol = protocol; 
}
    
void Response::set_status(std::string const &status)
{
    _status = status;
}

void Response::set_reason(std::string const &reason)
{
    _reason = reason;
}
