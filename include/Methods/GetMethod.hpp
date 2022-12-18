#pragma once

#include "AMethod.hpp"

/*	This class handles the behaviour of GET methods.	*/
class GetMethod : public Method
{
	// Member Variables
	private:
		std::string	_body_content;

	// Constructors
	public:
		GetMethod();
		~GetMethod();

	// Member Functions
	public:
		const std::string&	get_body_content(void);
		void				do_method(HandleRequest* hand_req, HandleResponse* hand_resp);
};