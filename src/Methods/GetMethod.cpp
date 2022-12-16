#include "../../include/Methods/GetMethod.hpp"

GetMethod::GetMethod()
{
    _body_content = "";
}

GetMethod::~GetMethod()
{

}

std::string const& GetMethod::get_body_content(void)
{
    return _body_content;
}
        
void GetMethod::do_method(HandleRequest *hand_req, HandleResponse *hand_resp)
{
    std::string path = "html_test";
    std::stringstream buffer;
    std::ifstream ifs;

    if (hand_req->get_request().get_path() == "/")
        path.append("/index.html");
    else
        path.append(hand_req->get_request().get_path());
    ifs.open(path);
    if (!ifs)
    {
        hand_resp->set_status_line("HTTP/1.1", "404", "Not Found");
        hand_resp->set_body("Error page");
        return ;
    }
    buffer << ifs.rdbuf();
    _body_content = buffer.str();
    if (_body_content.length())
    {
        hand_resp->set_status_line("HTTP/1.1", "200", "OK");
        hand_resp->make_header(_body_content);
        hand_resp->set_body(_body_content);
    }
}