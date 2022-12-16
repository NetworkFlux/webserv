#pragma once

#include "HandleResponse.hpp"
#include "HandleRequest.hpp"
#include "../Methods/GetMethod.hpp"
#include "../Methods/PostMethod.hpp"
#include "../utils.hpp"

class HandleHttp
{
    private:
        HandleRequest   * _req;
        HandleResponse  * _rep;

        Method          *_meth;

        std::string     final_line;
    
    public:
        HandleHttp();
        HandleHttp(HandleHttp const &x);
        HandleHttp& operator=(HandleHttp const &x);
        ~HandleHttp();

        std::string do_work(std::string req);
        void set_method(std::string meth);

    private:
        std::string get_reason_for_code(int code) const;
};