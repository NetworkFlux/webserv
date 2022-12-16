#include "../../include/Http/HandleResponse.hpp"

HandleResponse::HandleResponse(void)
{
    _response = new Response();
    _string_response = "";
}

HandleResponse::HandleResponse(HandleResponse const & x)
{
    *this = x;
}

HandleResponse::~HandleResponse(void) {
    delete _response;
}

HandleResponse & HandleResponse::operator=(HandleResponse const &x)
{
    if (!_response)
        _response = x._response;
    return *this;
}


void HandleResponse::make_string_response()
{
    std::map<std::string, std::string> header;
    std::map<std::string, std::string>::iterator it;

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

void HandleResponse::set_status_line(std::string protocol, std::string status, std::string reason)
{
    _response->set_protocol(protocol);
    _response->set_status(status);
    _response->set_reason(reason);
}


void HandleResponse::set_body(std::string body) {
    parse_body(body);
}


void HandleResponse::make_header(std::string const& body)
{
    std::string token;

    token.append("Content-Length: ");
    token.append(std::to_string(body.length()));
    parse_headers(token);
    token.clear();
    token.append("Content-Type: text/html");
    parse_headers(token);
}

std::string const &HandleResponse::get_string_response(void)
{
    return _string_response;
}

Response &HandleResponse::get_response(void)
{
    return *_response;
}