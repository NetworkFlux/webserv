#pragma once

#include "Messages/Request.hpp"
#include "Messages/Response.hpp"
#include "Config/ServerConfig.hpp"
#include "CGIServer.hpp"

class HandleHttp
{
	// Member Variables
	private:
		Request			_request;
		Response		_response;
		SimpleConfig	_config;
		std::string		_location;
		std::string		_req_path;
		bool		 	_directory_listing;
		std::string		_final_path;
		bool			_failed_request;

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
	bool	execute_cgi(void);
	bool	delete_file(void);
	void	redirection(SimpleConfig& loc_config);
	void	show_request(void);
	void	show_response(void);
	void	build_directory_listing();
	bool	client_close();
	bool	client_chunked();

	// Helper Functions
	private:
		void	find_location(void);
		bool	check_method_allowed(const std::vector<std::string>& loc_methods, const std::vector<std::string>& conf_methods, const std::string& asked_method);
		bool	check_root(const std::string& root);
		bool	check_index(SimpleConfig& loc, const std::vector<std::string>& conf_index);
		bool	is_index_file(SimpleConfig& loc, const std::vector<std::string>& conf_index);
		bool	check_max_body(long max_body, const std::string& body);
		bool	check_cgi_extension(const std::string& path);
		bool	check_protocol(void);
};