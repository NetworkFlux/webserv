#pragma once

#include "Response.hpp"
#include "HandleMessage.hpp"
#include "HandleRequest.hpp"

/*	This class is responsible for the respond message the server must send to
	the client. Of cours the response must be pretty specific and that is what this
	class is supposed to do.	*/
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
		Response&			get_response(void);
		const std::string&	get_string_response(void);

		void	set_status_line(std::string protocol, std::string status, std::string reason);
		void	set_body(std::string body);

	// Member Functions
	public:
		void	make_string_response(void);
		void	make_header(const std::string& body);
};