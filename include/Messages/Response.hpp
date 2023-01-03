#pragma once

#include "../utils.hpp"

/*
	This class is responsible for managing the response message the server will send.
	From the given member variables, it will generate the response message.
*/
class Response
{
	// Member Variables
	private:
		std::string							_status_line;
		std::map<std::string, std::string>	_header;
		std::vector<char>					_body;

		std::string		_protocol;
		size_t			_status_code;
		std::string		_status_message;

		size_t			_content_length;
		std::string		_content_type;

	// Constructors
	public:
		Response();
		Response(const std::string& protocol, size_t status_code, const std::string& status_message);
		~Response();

	// Operator Overload
	public:
		Response&	operator=(const Response& other);

	// Getter/Setter
	public:
		std::string								get_status_line(void) const;
		std::map<std::string, std::string>		get_header(void) const;
		std::vector<char>						get_body(void) const;

		std::string		get_protocol(void) const;
		size_t			get_status_code(void) const;
		std::string		get_status_message(void) const;

		void	set_status_line(const std::string& protocol, size_t status_code, const std::string& status_message);
		void	set_header(const std::map<std::string, std::string>& header);
		void	set_body(const std::vector<char>& body);

		void	set_content_length(size_t content_length);
		void	set_content_type(const std::string& content_type);

		void	set_protocol(const std::string& protocol);
		void	set_status_code(size_t status_code);
		void	set_status_message(const std::string& status_message);

	// Member Functions
	public:
		void	show_status_line(void) const;
		void	show_header(void) const;
		void	show_body(void) const;
		void	show_data(void) const;

		void	build_response(const std::string& path);
		std::string	give_response(void);


	// Helper Functions
	private:
		void	update_status_line(void);
		void	update_header(void);
};