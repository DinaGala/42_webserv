#include "Cgi.hpp"

Cgi::Cgi() {}

Cgi::Cgi(int port, const std::string &method)
{
	std::ostringstream oss;
	oss << port;
	this->_env["SERVER_PORT"] = oss.str();
	this->_env["SERVER_SOFTWARE"] = "webserver";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = "server";
	this->_env["REQUEST_METHOD"] = method;
	//this->_env["SCRIPT_NAME"] = "/cgi-bin/ubuntu_cgi_tester";
	this->_env["PATH_INFO"]="/";
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
	return ((void)c, *this);
}

void	Cgi::setUrl(const std::string &url)
{
	this->_url = "http://localhost:8080/cgi-bin/ubuntu_cgi_tester?input=hola";
	this->_searchFile(this->_parseUrl(url));
}

void	Cgi::setHost(const std::string &host)
{
	this->_env["HTTP_HOST"] = host;
}

void	Cgi::setServName(const std::string &serv)
{
	this->_env["SERVER_NAME"] = serv;
}

void	Cgi::addPair(const std::string &ext, const std::string &cmd)
{
	this->_pairs[ext] = cmd;
}
/*
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
}*/

//from map to char **
char	**Cgi::_getEnv(void)
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

void	Cgi::_setQueryString(std::vector<std::string>::iterator it,
							std::vector<std::string>::iterator end)
{
	while (it != end)
	{
		this->_env["QUERY_STRING"] = *it;
		it++;
	}
}

void	Cgi::_setPathInfo(std::vector<std::string>::iterator it,
						std::vector<std::string>::iterator end)
{
	if ((*it)[0] != '/')
	{
		if ((*it)[0] == '?')
			this->_setQueryString(it, end);
		return ;
	}
	while (it != end)
	{
		this->_env["PATH_INFO"] += *it + "/";
		it++;
	}
	this->_env["PATH_INFO"].erase(this->_env["PATH_INFO"].size() - 1);
}

void	Cgi::_searchFile(std::vector<std::string> vec)
{
	std::string	path = "";
	struct stat	path_stat;
	int	i, ret = -1;

	while (++i < vec.size())
	{
		path += vec[i].c_str();
		int ret = stat(path.c_str(), &path_stat);
		if (ret)
		{
			if (i == 0)
			{
				path = "";
				continue ;
			}
			else
				return ;
		}
		if (S_ISREG(path_stat.st_mode))
		{
			this->_env["SCRIPT_NAME"] = path;
			return ;
		}
		path += "/";
	}
	if (i < vec.size())
		this->_setPathInfo(vec.begin() + i, vec.end());
}

std::vector<std::string>	Cgi::_parseUrl(const std::string &url)
{
	std::vector<std::string>	split;
	std::string::size_type	first = url.find("//");
	std::string::size_type	second;

	first = 0;
	if (first != std::string::npos)
		first = 7;
	if (url[0] == '/')
		first = 1;
	while (1)
	{
		second = url.find("/", first);
		if (second == std::string::npos)
			break ;
		split.push_back(url.substr(first, second - first));
		first = second + 1;
	}
	split.push_back(url.substr(first));
	std::string	last = split.back();
	first = last.find("?");
	if (first != std::string::npos)
	{
		split.back() = last.substr(0, first);
		split.push_back(last.substr(first));
	}
	return (split);
}
/*
//needs access to host
void	Cgi::parseUrl(void)
{
	std::string::size_type	found;
	std::string	tmp = this->_url;

	found = tmp.find("//");
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
	if (url == "")
		return ;
	found = url.find("#");
	if (found == std::string::npos)
		this->_env["PATH_INFO"] = url;
	else
	{
		this->_env["PATH_INFO"] = tmp.substr(0, found);
		this->_env["QUERY_STRING"] = tmp.substr(found + 1);
	}
}
*/
#include <stdio.h>

std::string	Cgi::_childProcess(int *fdreq, int *fdcgi)
{
	//////////////////////////////////////////////////
    char **args = (char **)calloc(3, sizeof(char *));
    args[0] = (char *)calloc(30, sizeof(char));
    //args[1] = (char *)calloc(20, sizeof(char));
    const char tmp[] = "./cgi-bin/ubuntu_cgi_tester";
    strcpy(args[0], tmp);
	args[1] = NULL;
	this->_reqbody = "hola que tal";
	//////////////////////////////////////////////////
	if (dup2(fdreq[0], STDIN_FILENO) == -1 || dup2(fdcgi[1], STDOUT_FILENO) == -1)
	{
		perror("child: dup2");
		exit(1);
	}
	write(fdreq[1], this->_reqbody.c_str(), this->_reqbody.size());
	close(fdcgi[0]); // we don't want to read the cgi
	close(fdreq[1]); // we don't want to write in the request
	execve(args[0], args, this->_getEnv());
	close(fdreq[0]); // we don't want to read the request
	std::cerr << "Execve failed" << std::endl;
	close(fdcgi[1]);
	exit(1);
}

std::string	Cgi::executeCgi(void)
{
    pid_t pid;
    int status;
    int fdreq[2], fdcgi[2];
	std::string	cgi_response;

    if (pipe(fdreq) == -1 || pipe(fdcgi))
    {
        std::cerr << "Pipe failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
		this->_childProcess(fdreq, fdcgi);
    else
    {
        char buffer[1024];
        ssize_t count;
		
		if (waitpid(pid, &status, 0) == -1)
        {
            std::cerr << "Waitpid failed" << std::endl;
            exit(EXIT_FAILURE);
        }
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			return ("");
        close(fdreq[0]);
        close(fdcgi[1]);
        close(fdreq[1]);
        while ((count = read(fdcgi[0], buffer, sizeof(buffer))) != 0)
        {
            if (count == -1)
            {
                std::cerr << "Read failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            cgi_response += buffer;
        }
        close(fdcgi[0]);
    }
	std::cout << "cgi_response: " << cgi_response << std::endl;

    return (cgi_response);
}
