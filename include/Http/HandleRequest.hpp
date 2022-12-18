#pragma once

#include "Request.hpp"
#include "HandleMessage.hpp"

/*	This class is responsible for handeling the client requests. It should parse the
	request and make all the information in the request usable.	*/
class HandleRequest : public HandleMessage
{
	// Member Variables
	private:
		Request*	_request;
		int			_error_code;

	// Constructors
	public:
		HandleRequest();
		~HandleRequest();
	
	// Getters
	public:
		Request&	get_request(void);
		int			get_error_code(void);

	// Member Functions
	public:
		void	parse_request(const std::string& line);
		void	printRequest(void) const;

	// Helper Functions
	private:
		void	parse_start_line(const std::string& line);
		void	check_first_line(void);
};

#pragma once

#include "Response.hpp"
#include "HandleMessage.hpp"
#include "HandleRequest.hpp"

class HandleResponse : public HandleMessage
{
	// Member Variables
	private:
		Response*	_response;
		std::string	_string_response;

	// Constructors
	public:
		HandleResponse();
		~HandleResponse();

	// Getters/Setters
	public:
		Response&		get_respone(void);
		std::string&	get_string_response(void);

		void	set_status_line(std::string, std::string, std::string);
		void	set_body(std::string body);

	// Member Functions
	public:
		void	make_string_response(void);
		void	make_header(const std::string& body);
};