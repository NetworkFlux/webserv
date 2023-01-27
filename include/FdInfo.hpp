#pragma once

#include "utils.hpp"

struct FdInfo
{
	struct kevent events;
	bool close;
	std::string req;
	std::vector<std::string> chunks;
	bool chunked;
	size_t chunk_sent;
	size_t byte_sent; 
};