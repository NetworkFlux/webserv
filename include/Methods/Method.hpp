#pragma once

#include "../Http/HandleResponse.hpp"

#include <fstream>

class Method
{
    protected:
    public:
        Method();
        virtual ~Method();

        virtual void do_method(HandleRequest *handl_req, HandleResponse *hand_resp) = 0;
};