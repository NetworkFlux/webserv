#pragma once

#include "AMethod.hpp"

/*	This class handles the behaviour of POST methods.	*/
class PostMethod : public Method
{
	// Constructors
	public:
		PostMethod();
		~PostMethod();

	// Member Functions
	public:
		void	do_method(HandleRequest* hand_req, HandleResponse* hand_resp, ServerConfig* config);
};