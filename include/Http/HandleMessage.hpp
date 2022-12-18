#pragma once

#include "Message.hpp"

/*	This class will be responsible for handeling its message.	*/
class HandleMessage
{
	// Member Variables
	protected:
		Message*	_message;

	// Constructors
	public:
		HandleMessage();
		virtual ~HandleMessage();

	// Getters/Setters
	public:
		const std::map<std::string, std::string>&	get_message_header(void) const;
		const std::string&							get_message_body(void) const;
		Message*									get_message(void) const;

		void	set_message(Message* x);

	// Member Functions
	protected:
		void	parse_headers(std::string line);
		void	parse_body(std::string body);
};