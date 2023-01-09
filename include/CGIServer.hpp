#pragma once

#include "../include/utils.hpp"

class CGIServer
{
    
    // Member variables
    private:
        std::string         _cgiDir;
        std::string         _command;
        std::string         _query;
        std::string         _requestMethod;

    
        int                 _stdoutPipe[2];
        std::string         _responseBody;
        std::vector<char*>  _envc;
        

	// Constructors
    public:
        CGIServer(const std::string &cgiDir, const std::string &path, const std::string &requestMetho);
        ~CGIServer();

	// Member functions
    public:
        void setup_env(const std::map<std::string, std::string> &requestHeaders, const std::string &requestBody);
        void read_program();
        std::string run_program(const std::map<std::string, std::string> &requestHeaders, const std::string &requestBody);
};
