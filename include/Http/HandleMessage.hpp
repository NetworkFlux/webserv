#pragma once

#include "Message.hpp"

class HandleMessage
{

    protected:
		Message		*_message;

	public:
		HandleMessage(void);
		HandleMessage(HandleMessage const & x);
		virtual ~HandleMessage(void);
		HandleMessage &	operator=(HandleMessage const &x);


		const std::map<std::string, std::string> &get_message_header(void) const;
		const std::string &get_message_body(void) const;

		Message *get_message() const;
		void 	set_message(Message *x);

	protected:
		void parse_headers(std::string line);
		void parse_body(std::string body);
};