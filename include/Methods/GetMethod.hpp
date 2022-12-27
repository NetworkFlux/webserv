#pragma once

#include "AMethod.hpp"

/*	This class handles the behaviour of GET methods.	*/
class GetMethod : public Method
{
	// Member Variables
	private:
		std::string	_body_content;
		int			_errorCode;

	// Constructors
	public:
		GetMethod();
		~GetMethod();

	// Member Functions
	public:
		const std::string&	get_body_content(void);
		void				do_method(HandleRequest* hand_req, HandleResponse* hand_resp, ServerConfig* config);

	// Helper Functions
	private:
		void	locationSetup(ServerConfig& config, std::string* path, std::ifstream* ifs, HandleRequest* hand_req);
		bool	indexSetup(ServerConfig& config, std::string key, std::string* path);
		bool	errorCodeSetup(ServerConfig& config, std::string* path, HandleResponse* hand_resp);
};