#pragma once

#include "../utils.hpp"

/*	This base class Message contains a specific message that will be carry out to other
	classes depending on its use.	*/
class Message
{
	// Member Variables
	protected:
		std::map<std::string, std::string>	_headers;
		std::string							_body;

	// Constructors
	public:
		Message();
		virtual ~Message();

	// Getters/Setters
	public:
		std::string&								get_header(std::string key);
		const std::map<std::string, std::string>&	get_headers(void) const;
		const std::string&							get_body(void) const;

		void	set_body(const std::string& body);

	// Member Functions
	public:
		void	add_header(std::string key, const std::string& content);

};

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
