#include "../../include/Http/HandleHttp.hpp"

HandleHttp::HandleHttp()
{
    _req = new HandleRequest();
    _rep = new HandleResponse();
    final_line = "";
}


HandleHttp::HandleHttp(HandleHttp const &x)
{
    *this = x;
}

HandleHttp& HandleHttp::operator=(HandleHttp const &x)
{
    if (this != &x)
    {
        _req = x._req;
        _rep = x._rep;
    }
    return *this;
}

/* This function receive a string request and output the string response */
std::string HandleHttp::do_work(std::string str_req)
{
    _req->parse_request(str_req);
    // _req->printRequest();
    
    if (_req->get_error_code() != 0)
        _rep->set_status_line("HTTP/1.1", toString(_req->get_error_code()), get_reason_for_code(_req->get_error_code()));
    else
    {
        set_method(_req->get_request().get_method());
        _meth->do_method(_req, _rep);
    }

    _rep->make_string_response();
    // std::cout << _rep->get_string_response() << std::endl;

    return (_rep->get_string_response());
}

void HandleHttp::set_method(std::string meth)
{
    if (meth == "GET")
        _meth = new GetMethod();
    else if (meth == "POST")
        _meth = new PostMethod();
    //else if (meth == "DELETE")
    //     _meth = new DeleteMethod();
}

std::string HandleHttp::get_reason_for_code(int code) const
{
    std::string ret;

    if (code == 501)
        ret = "Not Implemented";
    if (code == 505)
        ret = "HTTP Version Not Supported";
    return (ret);
}

HandleHttp::~HandleHttp()
{
    delete _req;
    delete _rep;
}