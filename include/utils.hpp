#pragma once

#define RESET	"\033[m"
#define PURPLE	"\033[0;35m"
#define BLUE	"\033[0;34m"
#define YELLOW	"\033[0;33m"
#define GREEN	"\033[0;32m"
#define RED		"\033[0;31m"
#define GREY	"\033[0;30m"


#define RESET_B		"\033[m"
#define PURPLE_B	"\033[1;35m"
#define BLUE_B		"\033[1;34m"
#define YELLOW_B	"\033[1;33m"
#define GREEN_B		"\033[1;32m"
#define RED_B		"\033[1;31m"
#define GREY_B		"\033[1;30m"

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>


// Utils Functions
bool	charInStr(const char& c, const std::string& str);