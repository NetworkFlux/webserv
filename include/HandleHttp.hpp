#pragma once

#include "Messages/Request.hpp"
#include "Messages/Response.hpp"
#include "Config/ServerConfig.hpp"

/*	This class is responsible for handeling requests and responses.
	It will be given the client request and must build a response depending
	on what is allowed by the configuration file.
*/

class HandleHttp
{
	// Member variables
	private:
		Request			_request;
		Response		_response;
		ServerConfig*	_serverConfig;
		std::string		_final_path;

	// Constructors
	public:
		HandleHttp(const std::string& request_line, ServerConfig* serverConfig);
		~HandleHttp();

	// Getters/Setters
	public:
		Response&	get_response(void);

	// Member Functions
	public:
		void		do_work(void);

		bool	check_method_allowed(const std::vector<std::string>& methods, const std::string& method);
		bool	check_root(const std::string& root);
		bool	check_index(const std::vector<std::string>& index);

		void	show_request(void);
		void	show_response(void);
		void	show_response_status_line(void);

	// Helper Functions
	private:
		
};