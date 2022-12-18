#include "../../include/Http/HandleHttp.hpp"

/*	Initialize the member variables.	*/
HandleHttp::HandleHttp()
{
	_req = new HandleRequest();
	_resp = new HandleResponse();
	_final_line = "";
}

HandleHttp::~HandleHttp()
{
	delete _req;
	delete _resp;
}

/*	Sets the right method based on the client request.	*/
void	HandleHttp::set_method(std::string meth)
{
	if (meth == "GET")
		_meth = new GetMethod();
	else if (meth == "POST")
		_meth = new PostMethod();
}

/*	This function does all the work needed to make a acceptable response based
	on the client request.	*/
std::string	HandleHttp::do_work(std::string req)
{
	_req->parse_request(req);
	if (_req->get_error_code() != 0)
		_resp->set_status_line("HTTP/1.1", toString(_req->get_error_code()), get_reason_for_code(_req->get_error_code()));
	else
	{
		set_method(_req->get_request().get_method());
		_meth->do_method(_req, _resp);
	}
	_resp->make_string_response();
	return (_resp->get_string_response());
}

/*	Provides an explanation on the error code. For troubleshooting issues.	*/
std::string	HandleHttp::get_reason_for_code(int code) const
{
	std::string ret;

	if (code == 501)
		ret = "Not Implemented";
	if (code == 505)
		ret = "HTTP Version Not Supported";
	return (ret);
}