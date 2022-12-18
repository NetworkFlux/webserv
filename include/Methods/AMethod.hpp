#pragma once

#include <fstream>

#include "../Http/HandleResponse.hpp"

/*	This abstract Method class will be the base template for every methods.	*/
class Method
{
	// Constructors
	public:
		Method();
		virtual ~Method();

	// Member Functions
	public:
		virtual void do_method(HandleRequest *handl_req, HandleResponse *hand_resp) = 0;
};