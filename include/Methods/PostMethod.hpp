#pragma once

#include "Method.hpp"

class PostMethod : public Method
{
    private:

    public:
        PostMethod();
        ~PostMethod();

        void do_method(HandleRequest *hand_req, HandleResponse *hand_resp);
};
