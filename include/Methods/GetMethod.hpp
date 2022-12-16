#pragma once

#include "Method.hpp"

class GetMethod : public Method
{
    private:
        std::string _body_content;

    public:
        GetMethod();
        ~GetMethod();
        
        std::string const& get_body_content(void);
        void do_method(HandleRequest *hand_req, HandleResponse *hand_resp);
};
