#pragma once

#include "../utils.hpp"

/*
	This class is responsible for managing the request message the client sent.
	It will parse the message and store the informations in the member variables.
*/
class Request
{
	// Member Variables
	private:
		std::string							_status_line;
		std::map<std::string, std::string>	_header;
		std::string							_body;

		std::string							_method;
		std::string							_path;
		std::string							_protocol;

	// Constructors
	public:
		Request();
		Request(const std::string& status_line);
		~Request();

	// Operator Overload
	public:
		Request&	operator=(const Request& other);

	// Getters/Setters
	public:
		const std::string&	get_status_line(void) const;
		const std::string&	get_method(void) const;
		const std::string&	get_path(void) const;
		const std::string&	get_protocol(void) const;
		const std::string&	get_body(void) const;
		const std::map<std::string, std::string>&	get_header(void) const;

		void	set_status_line(const std::string& status_line);
		void	set_method(const std::string& method);
		void	set_path(const std::string& path);
		void	set_protocol(const std::string& protocol);
		void	set_header(const std::string& request);
		void	set_body(const std::string& str);

	// Helper Functions
	public:
		void	show_data(void) const;
};