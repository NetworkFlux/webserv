#include "../../include/Config/SimpleConfig.hpp"

/*	This constructor only initialize the variable for later use.	*/
SimpleConfig::SimpleConfig() : _listen(), _serverNames(), _errorPages(), _autoIndex(), _index()
{

}

/*	Deletes the values stored in the member variable after some necessary steps.	*/
SimpleConfig::~SimpleConfig()
{

}