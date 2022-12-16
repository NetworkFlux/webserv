#pragma once

#include "Message.hpp"


class Response : public Message
{
    
    private:
        std::string _protocol;
        std::string _status;
        std::string _reason;
    public:
        Response();
        Response(Response const &x);
        ~Response();
    
        std::string const &get_protocol(void);
        std::string const &get_status(void);
        std::string const &get_reason(void);

        void set_protocol(std::string const &method);
        void set_status(std::string const &uri);
        void set_reason(std::string const &version);

};