#pragma once

#include "Messages/Request.hpp"
#include "Messages/Response.hpp"
#include "Config/ServerConfig.hpp"

class HandleHttp
{
	// Member Variables
	private:
		Request			_request;
		Response		_response;
		SimpleConfig	_config;
		std::string		_location;
		std::string		_req_path;
		std::string		_final_path;

	// Constructors
	public:
		HandleHttp(const std::string& request_line, ServerConfig* serverConfig, size_t serv_index);
		~HandleHttp();

	// Getter/Setters
	public:
		Response&	get_response(void);

	// Member Functions
	public:
	void	do_work(void);
	void	build_response(SimpleConfig& loc_config);
	void	show_request(void);
	void	show_response(void);

	// Helper Functions
	private:
		void	find_location(void);
		bool	check_method_allowed(const std::vector<std::string>& loc_methods, const std::vector<std::string>& conf_methods, const std::string& asked_method);
		bool	check_root(const std::string& root);
		bool	check_index(const std::vector<std::string>& loc_index, const std::vector<std::string>& conf_index);

};