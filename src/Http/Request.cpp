#include "../include/Http/Request.hpp"

Request::Request()
{
    _method = "";
    _path  = "";
    _protocol = "";
}

Request::Request(Request const &x)
{
    *this = x;
}

Request::~Request()
{

}

void Request::set_method(std::string const &method)
{
    _method = method;
}

void Request::set_path(std::string const &path)
{
    _path = path;
}

void Request::set_protocol(std::string const &protocol)
{
    _protocol = protocol;
}

std::string const &Request::get_method(void)
{
    return _method;
}

std::string const &Request::get_path(void)
{
    return _path;
}
std::string const &Request::get_protocol(void)
{
    return _protocol;
}
