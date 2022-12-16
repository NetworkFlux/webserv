#pragma once

#include "Message.hpp"

class Request : public Message
{
    private:
        std::string _method;
        std::string _path;
        std::string _protocol;

    public:
        Request();
        Request(Request const &x);
        ~Request();

        std::string const &get_method(void);
        std::string const &get_path(void);
        std::string const &get_protocol(void);

        void set_method(std::string const &method);
        void set_path(std::string const &uri);
        void set_protocol(std::string const &version);


};