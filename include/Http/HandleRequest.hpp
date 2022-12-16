#pragma once

#include "Request.hpp"
#include "HandleMessage.hpp"

class HandleRequest : public HandleMessage
{
    private:
		Request *	_request;
		int			_error_code;

	public:
		HandleRequest(void);
		HandleRequest(HandleRequest const & x);
		~HandleRequest(void);
		HandleRequest &	operator=(HandleRequest const &x);

		void 	parse_request(std::string const &line);
		
		Request &get_request(void);
		int		get_error_code(void) const;

		void printRequest(void) const;

	private:
		void parse_start_line(std::string const &line);
		void check_first_line(void);

};