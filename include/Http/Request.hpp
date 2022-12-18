#pragma once

#include "Message.hpp"

/*	This class will hold all the informations required for a client request to
	the server.	*/
class Request : public Message
{
	// Member Variables
	private:
		std::string	_method;
		std::string	_path;
		std::string	_protocol;

	// Constructors
	public:
		Request();
		~Request();

	// Getters/Setters
	public:
		const std::string&	get_method(void);
		const std::string&	get_path(void);
		const std::string&	get_protocol(void);

		void	set_method(const std::string& method);
		void	set_path(const std::string& uri);
		void	set_protocol(const std::string& version);
};