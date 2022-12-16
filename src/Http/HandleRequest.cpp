#include "../../include/Http/HandleRequest.hpp"

HandleRequest::HandleRequest(void)
{
    _request = new Request();
    _error_code = 0;
}

HandleRequest::HandleRequest(HandleRequest const & x)
{
    *this = x;
}

HandleRequest& HandleRequest::operator=(HandleRequest const &x)
{
    if (!_request)
        _request = x._request;
    return *this;
}

HandleRequest::~HandleRequest(void)
{
    delete _request;
}


void HandleRequest::parse_request(std::string const &line)
{
    std::stringstream ss(line);
    std::string token;
    bool first = false;

    parse_start_line(line);
    while(std::getline(ss, token, '\n'))
    {
        if (token == "\r")
            break ;
        if (first)
            parse_headers(token);
        first = true;
    }
    ss >> token;
    parse_body(token);
}

void HandleRequest::parse_start_line(std::string const &line)
{
    std::stringstream ss(line);
    std::string buff;

    ss >> buff;
    _request->set_method(buff);
    ss >> buff;
    _request->set_path(buff);
    ss >> buff;
    _request->set_protocol(buff);

    check_first_line();
}

void HandleRequest::check_first_line(void)
{
    if (_request->get_method() != "GET" && _request->get_method() != "POST" && _request->get_method() != "DELETE")
        _error_code = 501;
    if (_request->get_protocol() != "HTTP/1.1" && _request->get_protocol() != "HTTP/1.0")
        _error_code = 505;
}

Request &HandleRequest::get_request(void)
{
    return *_request;
}

int HandleRequest::get_error_code(void) const
{
    return _error_code;
}

/* Print the parsed request for debugging purpose */
void HandleRequest::printRequest(void) const
{
    std::cout <<"--Client request --" << std::endl << std::endl;
    std::cout << RED_B << "Method : " << _request->get_method() << std::endl;
    std::cout << "Path : " <<_request->get_path() << std::endl;
    std::cout << "HTTP Version : " << _request->get_protocol() << RESET << std::endl;

    std::map<std::string, std::string> mapp = get_message_header();
    std::map<std::string, std::string>::iterator it;
    std::cout << "Headers " << GREEN_B << std::endl;
    for (it = mapp.begin(); it != mapp.end(); it++)
    {
        std::cout << it->first << " : ";
        std::cout << it->second << std::endl;
    }
    std::cout << RESET;
    std::cout << "Body " << std::endl;
    std::cout << YELLOW_B << get_message_body() << RESET << std::endl;

}