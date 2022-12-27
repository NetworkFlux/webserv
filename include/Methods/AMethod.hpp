#pragma once

#include <fstream>

#include "../Http/HandleResponse.hpp"
#include "../Config/ServerConfig.hpp"

/*	This abstract Method class will be the base template for every methods.	*/
class Method
{
	// Constructors
	public:
		Method();
		virtual ~Method();

	// Member Functions
	public:
		virtual void do_method(HandleRequest* hand_req, HandleResponse* hand_resp, ServerConfig* config) = 0;
};