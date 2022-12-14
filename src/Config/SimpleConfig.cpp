#include "../../include/Config/SimpleConfig.hpp"

/*	This constructor only initialize the variable for later use.	*/
SimpleConfig::SimpleConfig() : _listen(), _serverNames(), _errorPages(), _index(), _root(),
	_maxBody(0), _methods(), _uploadPath(), _cgiExt(), _cgiPath(), _locations()
{
	_autoIndex = false;
	_upload = false;
}

/*	Deletes the values stored in the member variable after some necessary steps.	*/
SimpleConfig::~SimpleConfig()
{

}