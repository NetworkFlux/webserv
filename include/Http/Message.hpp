#pragma once

#include "../utils.hpp"

class Message
{

    protected:
        std::map<std::string, std::string>  _headers;
        std::string                         _body;

    public:
        Message();
        Message(Message const &x);
        Message &operator=(Message const &x);
        virtual ~Message(void);

        std::string & get_header(std::string key);
        const std::map<std::string, std::string> &get_headers(void) const;
        const std::string &get_body(void) const;

        void add_header(std::string key, const std::string &content);
        void set_body(const std::string & body);

};