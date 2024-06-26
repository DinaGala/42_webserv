#include "Cgi.hpp"

Cgi::Cgi() {}

Cgi::Cgi(const std::string &serv, int port, const std::string &method,
		const std::string &host)
{
	this->_env["SERVER_SOFTWARE"] = "webserver";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = serv;
	this->_env["REQUEST_METHOD"] = method;
	this->_env["HTTP_HOST"] = host;
	std::ostringstream oss;
	oss << port;
	this->_env["SERVER_PORT"] = oss.str();
	this->_env["PATH_INFO"]="/";
	this->_env["PATH_TRANSLATED"]="./";
	///////// 	TMP VARS   /////////////
	this->_pairs[".sh"] = "/bin/bash";
	this->_pairs[".js"] = "/usr/bin/node";
	this->_pairs[".php"] = "/usr/bin/php";
	this->_pairs[".py"] = "/usr/bin/python3";
	///////////////////////////////////
}

Cgi::Cgi(const Cgi &c)
{
	(void)c;
}

Cgi::~Cgi() {}

Cgi	&Cgi::operator=(const Cgi &c)
{
	(void)c;
	return (*this);
}

void	Cgi::addPair(const std::string &ext, const std::string &cmd)
{
	this->_pairs[ext] = cmd;
}

char	**Cgi::vecToMat(const std::vector<std::string> &vec)
{
	char	**mat;
	size_t	len = vec.size();

	try
	{
		mat = new char *[len + 1];
	}
	catch (std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < len; i++)
	{
		try
		{
			mat[i] = new char [vec[i].length() + 1];
			std::strcpy(mat[i], vec[i].c_str());
		}
		catch (std::exception &ex)
		{
			std::cerr << ex.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return (mat);
}

char	**Cgi::getEnv(void)
{
	char	**mat;
	std::string	tmp;
	std::map<std::string, std::string>::iterator it = this->_env.begin();
	std::map<std::string, std::string>::iterator end = this->_env.end();

	try
	{
		mat = new char *[this->_env.size() + 1];
	}
	catch (std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	int i = 0;
	while (it != end)
	{
		try
		{
			tmp = it->first + "=" + it->second;
			mat[i] = new char [tmp.size() + 1];
			std::strcpy(mat[i], tmp.c_str());
		}
		catch (std::exception &ex)
		{
			std::cout << ex.what() << std::endl;
			for (int j = 0; j < i; ++j)
                delete[] mat[j];
            delete[] mat;
            exit(EXIT_FAILURE);
		}
		it++;
		i++;
	}
	mat[i] = NULL;
	return (mat);
}

void	Cgi::setContentLength(const std::string &clen = "0")
{
	this->_env["CONTENT_LENGTH"] = clen;
}

void	Cgi::setContentType(const std::string &ctype = "")
{
	this->_env["CONTENT_TYPE"] = ctype;
}

//needs access to host
void	Cgi::parseUrl(const std::string &url)
{
	std::string	tmp = url;
	size_t		found;

	found = url.find("//");
	if (found != std::string::npos)
		tmp = tmp.substr(found + 2);
	tmp = tmp.substr(this->_env["HTTP_HOST"].length());
	//this for should be done in the request probably
	found = std::string::npos;
	for (std::map<std::string, std::string>::iterator it = this->_pairs.begin();
			it != this->_pairs.end() || found != std::string::npos; ++it)
		found = tmp.find(it->first);
	if (found == std::string::npos)
		return ;
	this->_env["SCRIPT_NAME"] = tmp.substr(0, tmp.find("/", found));
	tmp = tmp.substr(this->_env["SCRIPT_NAME"].length());
	found = tmp.find("#");
	if (found == std::string::npos)
		this->_env["PATH_INFO"] = tmp;
	else
	{
		this->_env["PATH_INFO"] = tmp.substr(0, found);
		this->_env["QUERY_STRING"] = tmp.substr(found + 1);
	}
}

#include <stdio.h>
int Cgi::executeCgi(void)
{
    pid_t pid;
    int status;
    int pipefd[2];
	std::string	cgi_response;

    if (pipe(pipefd) == -1)
    {
        std::cerr << "Pipe failed" << std::endl;
        exit(EXIT_FAILURE);
    }
	//////////////////////////////////////////////////
    char **args = (char **)calloc(3, sizeof(char *));
    args[0] = (char *)calloc(20, sizeof(char));
    args[1] = (char *)calloc(20, sizeof(char));
    const char tmp[] = "./cgi/test";
    strcpy(args[0], tmp);
	//////////////////////////////////////////////////
    pid = fork();
    if (pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "dup2 failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        execve(args[0], args, this->getEnv());
        std::cerr << "Execve failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);
        char buffer[1024];
        ssize_t count;
		
		std::cout << pid << std::endl;
		if (waitpid(pid, &status, 0) == -1)
        {
            std::cerr << "Waitpid failed" << std::endl;
            exit(EXIT_FAILURE);
        }
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			return (status);
        while ((count = read(pipefd[0], buffer, sizeof(buffer))) != 0)
        {
            if (count == -1)
            {
                std::cerr << "Read failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            cgi_response += buffer;
        }
        close(pipefd[0]);
    }

    free(args[0]);
    free(args);

	std::cout << "cgi_response: " << cgi_response << std::endl;

    return (0);
}
