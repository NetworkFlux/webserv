#pragma once

#include "Response.hpp"
#include "HandleRequest.hpp"
#include "HandleMessage.hpp"


class HandleResponse : public HandleMessage
{
    private:
		Response	*_response;
		std::string _string_response;

	public:
		HandleResponse(void);
		HandleResponse(HandleResponse const & x);
		~HandleResponse(void);
		HandleResponse &	operator=(HandleResponse const &x);

		void set_status_line(std::string , std::string, std::string);
		void set_body(std::string body);
		void make_string_response();

		void make_header(std::string const& body);

		std::string const &get_string_response(void);
		Response &get_response(void);
};
