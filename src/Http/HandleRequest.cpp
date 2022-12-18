#include "../../include/Http/HandleRequest.hpp"

/*	Initialize the member variables.	*/
HandleRequest::HandleRequest()
{
	_request = new Request();
	_error_code = 0;
}

HandleRequest::~HandleRequest()
{
	delete _request;
}

/* Return the required request.	*/
Request&	HandleRequest::get_request(void)
{
	return (*_request);
}

/*	Return the error_code.	*/
int	HandleRequest::get_error_code(void)
{
	return (_error_code);
}

/*	This function checks for errors in the client request. It also will parse the header
	informations in the request to set everything up for use.	*/
void	HandleRequest::parse_request(const std::string& line)
{
	std::stringstream	ss(line);
	std::string			token;
	bool				first = false;

	parse_start_line(line);
	while (getline(ss, token, '\n'))
	{
		if (token == "\r")
			break ;
		if (first)
			parse_headers(token);
		first = true;
	}
	ss >> token;
	parse_body(token);
}

/*	Outputs all the informations contain in the Request.	*/
void	HandleRequest::printRequest(void) const
{
	std::cout << "--Client Request--" << std::endl << std::endl;
	std::cout << RED_B << "Method : " << _request->get_method() << std::endl;
	std::cout << "Path : " << _request->get_path() << std::endl;
	std::cout << "HTTP Version : " << _request->get_protocol() << RESET << std::endl;

	std::map<std::string, std::string>	mapp = get_message_header();
	std::map<std::string, std::string>::iterator	it;
	std::cout << "Headers " << GREEN_B << std::endl;
	for (it = mapp.begin(); it != mapp.end(); ++it)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << RESET;
	std::cout << "Body " << std::endl;
	std::cout << YELLOW_B << get_message_body() << RESET << std::endl;
}

/*	This function will parse the first line of the request to setup the
	required Request parameters.	*/
void HandleRequest::parse_start_line(const std::string& line)
{
	std::stringstream	ss(line);
	std::string			buff;

	ss >> buff;
	_request->set_method(buff);
	ss >> buff;
	_request->set_path(buff);
	ss >> buff;
	_request->set_protocol(buff);

	check_first_line();
}

/*	This function checks whether there are errors in the first line of the clien
	request.	*/
void	HandleRequest::check_first_line(void)
{
	if (_request->get_method() != "GET" && _request->get_method() != "POST" && _request->get_method() != "DELETE")
		_error_code = 501;
	if (_request->get_protocol() != "HTTP/1.1" && _request->get_protocol() != "HTTP/1.0")
		_error_code = 505;
}