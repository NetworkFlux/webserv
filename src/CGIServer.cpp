#include "../include/CGIServer.hpp"

CGIServer::CGIServer(const std::string &cgiDir, const std::string &path, const std::string &requestMethod) : _cgiDir(cgiDir), _requestMethod(requestMethod)
{
    if (_requestMethod == "GET")
    {
        std::size_t pos = path.find('?');
        std::string rpath = path.substr(0, pos);
        _query = path.substr(pos + 1);
        _command = _cgiDir + rpath;
    }
    else
        _command = _cgiDir + path;
}

CGIServer::~CGIServer()
{}


/* Set up the environment variables */
void CGIServer::setup_env(const std::map<std::string, std::string> &requestHeaders,  const std::string &requestBody)
{
    std::vector<std::string> env;

    if (_requestMethod == "GET")
        env.push_back("QUERY_STRING=" + _query);
    env.push_back("PATH_INFO=" + _command);
    env.push_back("REQUEST_METHOD=" + _requestMethod);
    env.push_back("CONTENT_LENGTH=" + std::to_string(requestBody.size()));

    if (_requestMethod == "POST")
    {
        std::map<std::string, std::string> header = requestHeaders;
        std::string c_type = header["Content-Type"];
        env.push_back("CONTENT_TYPE=" + c_type);
    }
    for (std::map<std::string, std::string>::const_iterator it = requestHeaders.begin(); it != requestHeaders.end(); ++it)
    {
        std::string envName = "HTTP_" + it->first;
        std::replace(envName.begin(), envName.end(), '-', '_');
        env.push_back(envName + '=' + it->second);
    }

    for (unsigned int i = 0; i < env.size(); ++i)
        _envc.push_back(const_cast<char*>(env[i].c_str()));
    _envc.push_back(nullptr);
}

/* Read the program's output from the pipes */
void CGIServer::read_program(std::string *response)
{
    char buffer[1024];
    while (int bytesRead = read(_outpipe[0], buffer, sizeof(buffer)))
    {
        if (bytesRead < 0)
        {
            std::cerr << "Error reading from CGI stdout: " << strerror(errno) << std::endl;
            break;
        }
        response->append(buffer, bytesRead);
    }
}

/* Exeute CGI program */
int CGIServer::run_program(const std::map<std::string, std::string> &requestHeaders, const std::string &requestBody, std::string *response)
{
    pipe(_outpipe);
    if (_requestMethod == "POST")
        pipe(_inpipe);

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        setup_env(requestHeaders, requestBody);

        // Write the request body to STDIN so it can be read by the cgi
        if (_requestMethod == "POST") {
            close(_inpipe[1]);
            dup2(_inpipe[0], 0);
        }

        close(_outpipe[0]);
        dup2(_outpipe[1], STDOUT_FILENO);
        
        execve(_command.c_str(), nullptr, _envc.data());

        std::cerr << "Error executing CGI program: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        close(_outpipe[1]);
        if (_requestMethod == "POST") {
            close(_inpipe[0]);
            size_t ret;
            std::cerr << "oui" << std::endl;
			ret = write(_inpipe[1], requestBody.c_str(), requestBody.size());
			std::cerr << ret << "et " << strerror(errno) << std::endl;
            close(_inpipe[1]); // send EOF
        }

        read_program(response);
        close(_outpipe[0]);

        // Wait for the child process to exit
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exitCode = WEXITSTATUS(status);
            if (exitCode != 0)
            {
                std::cerr << "CGI program exited with non-zero status: " << exitCode << std::endl;
                return (-1);
            }
        }
        else
        {
            std::cerr << strerror(errno) << std::endl;
            std::cerr << "CGI program did not exit normally" << std::endl;
            return (-1);
        }
    }
    else
    {
        std::cerr << "Error forking process: " << strerror(errno) << std::endl;
        return (-1);
    }

    return(1);
}