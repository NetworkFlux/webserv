#pragma once

#include "HandleResponse.hpp"
#include "HandleResponse.hpp"
#include "../Methods/GetMethod.hpp"
#include "../Methods/PostMethod.hpp"
#include "../utils.hpp"

/*	This class contains all the necessary information needed for requests aswell as
	for responses. It is this object that does all the work and the communication
	between server and client.	*/
class HandleHttp
{
	// Member Variables
	private:
		HandleRequest*	_req;
		HandleResponse*	_resp;
		Method*			_meth;
		std::string		_final_line;

	// Constuctors
	public:
		HandleHttp();
		~HandleHttp();

	// Setters
	public:
		void	set_method(std::string meth);

	// Member Functions
	public:
		std::string	do_work(std::string req);

	// Helper Functions
	private:
		std::string	get_reason_for_code(int code) const;
};