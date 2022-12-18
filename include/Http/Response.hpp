#pragma once

#include "Message.hpp"

/*	This class will hold all the informations required for a server response to
	the client.	*/
class Response : public Message
{
	// Member Variables
	private:
		std::string	_protocol;
		std::string	_status;
		std::string	_reason;

	// Constructors
	public:
		Response();
		~Response();

	// Getters/Setters
	public:
		const std::string&	get_protocol(void);
		const std::string&	get_status(void);
		const std::string&	get_reason(void);

		void	set_protocol(const std::string& version);
		void	set_status(const std::string& status);
		void	set_reason(const std::string& reason);
};

