#include "Cgi.hpp"

static void	error(int sock, const char *ft, const std::string &msg)
{
	std::cerr << "Error: " << ft << ": " << msg << std::endl;
	if (close(sock))
		std::cerr << "Failed to close socket" << std::endl;
	exit(EXIT_FAILURE);
}

Cgi::Cgi() {}

Cgi::Cgi(int port, const std::string &method, int socket): _socket(socket)
{
	std::ostringstream oss;
	oss << port;
	this->_env["SERVER_PORT"] = oss.str();
	this->_env["SERVER_SOFTWARE"] = "webserver";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = "server";
	this->_env["REQUEST_METHOD"] = method;
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

void	Cgi::setEnvVars(const std::string &url, const std::string &host, const std::string &serv)
{
	(void)url;
	//this->_url = "http://localhost:8080/cgi-bin/ubuntu_cgi_tester?input=hola";
	this->_url = "http://localhost:8080/cgi-bin/test.py";
	std::vector<std::string> vec = this->_parseUrl(this->_url);
	this->_searchFile(vec);
	//this->_searchFile(this->_parseUrl(this->_url));
	this->_env["HTTP_HOST"] = host;
	this->_env["SERVER_NAME"] = serv;
}

void	Cgi::addPair(const std::string &ext, const std::string &cmd)
{
	this->_pairs[ext] = cmd;
}

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
			mat[i] = strdup(tmp.c_str());
			if (!mat[i])
				throw std::bad_alloc();
		}
		catch (std::exception &ex)
		{
			std::cerr << ex.what() << std::endl;
			for (int j = 0; j < i; ++j)
                delete[] mat[j];
            delete[] mat;
			exit(1);
		}
		it++;
		i++;
	}
	mat[i] = NULL;
	return (mat);
}

char	**Cgi::vecToMat(const std::vector<std::string> &vec)
{
	char	**mat;
	size_t	len = vec.size();

	try
	{
		mat = new char *[len + 1];
		mat[len] = NULL;
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
			mat[i] = new char [vec[i].size() + 1];
			std::strcpy(mat[i], vec[i].c_str());
			mat[i][vec[i].size()] = '\0';
		}
		catch (std::exception &ex)
		{
			std::cerr << ex.what() << std::endl;
			for (size_t j = 0; j < i; ++j)
                delete[] mat[j];
            delete[] mat;
			exit(EXIT_FAILURE);
		}
	}
	return (mat);
}

void	Cgi::_setQueryString(std::vector<std::string>::iterator it,
							std::vector<std::string>::iterator end)
{
	this->_env["QUERY_STRING"] = &((*it).c_str())[1];
	it++;
	while (it != end)
	{
		this->_env["QUERY_STRING"] = *it;
		it++;
	}
}

void	Cgi::_setPathInfo(std::vector<std::string>::iterator it,
						std::vector<std::string>::iterator end)
{
	if ((*it)[0] == '?')
	{
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
	std::vector<std::string>::size_type	i = -1;
	int ret;

	while (++i < vec.size())
	{
		path += vec[i].c_str();
		ret = stat(path.c_str(), &path_stat);
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
			break ;
		}
		path += "/";
	}
	if (i < vec.size() - 1)
		this->_setPathInfo(vec.begin() + i + 1, vec.end());
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

std::vector<std::string> Cgi::getArgs(void)
{
		std::vector<std::string> args;
		std::string::size_type found = this->_env["SCRIPT_NAME"].find(".");
		std::string	ext;

		if (found != std::string::npos)
		{
			ext = this->_env["SCRIPT_NAME"].substr(found);
			if (this->_pairs.find(ext) == this->_pairs.end())
				error(this->_socket, "cgi", "extension " + ext + " not allowed");
			args.push_back(this->_pairs[ext]);
		}
		args.push_back(this->_env["SCRIPT_NAME"]);
		return (args);
}

#include <stdio.h>
void	Cgi::_childProcess(int *req, int *cgi)
{
	char	**args = this->vecToMat(this->getArgs());
	char	**env = this->_getEnv();
	if (close(req[1]) || close(cgi[0]))
		error(this->_socket, "close", "failed to close pipe in child process");
	if (dup2(req[0], STDIN_FILENO) == -1 || dup2(cgi[1], STDOUT_FILENO) == -1)
		error(this->_socket, "dup2", "failed to redirect fd in child process");
	if (close(req[0]) || close(cgi[1]))
		error(this->_socket, "close", "failed to close pipe in child process");
//	int i = 0;
	//for (char **env = this->_getEnv(); env[i]; i++)
	//	std::cerr << "\t|" << env[i] << "|" << std::endl;
	//execve(args[0], args, NULL);
	for (int i = 0; args[i]; i++)
		std::cerr << "\t" << args[i] << std::endl;
	execve(args[0], args, env);
	perror("execve");
	error(this->_socket, "execve", "failed to execute" + this->_env["SCRIPT_NAME"]);
}

std::string	Cgi::executeCgi(void)
{
	pid_t	pid;
	int		status;
	int		req[2], cgi[2];
	std::string	cgi_response;
	char buffer[1024];
	ssize_t count;

	//this->_reqbody = "[CGI: REQUEST BODY]";
	if (pipe(req) || pipe(cgi))
		error(this->_socket, "pipe", "unable to create a pipe");
	write(req[1], this->_reqbody.c_str(), this->_reqbody.size());
	write(req[1], "\0", 1);
	pid = fork();
	if (pid == -1)
		error(this->_socket, "fork", "unable to create a new process");
	if (pid == 0)
		this->_childProcess(req, cgi);
	if (close(req[0]) || close(cgi[1]))
		std::cerr << "Error: close" << std::endl;
	if (close(req[1]))
		error(this->_socket, "close", "failed to close pipe");
	if (waitpid(pid, &status, 0) == -1)
		error(this->_socket, "waitpid", "something went wrong");
	//int i = 0;
	//for (char **env = this->_getEnv(); env[i]; i++)
	//	std::cout << env[i] << "\n";
	while ((count = read(cgi[0], buffer, sizeof(buffer))) != 0)
	{
		if (count == -1)
			error(this->_socket, "read", "couldn't read cgi's respone");
		buffer[count] = '\0';
		//std::cerr << "\033[1;31mcount: " << count << " buffer: " << buffer << "\033[0m" << std::endl;
		cgi_response += buffer;
	}
	if (close(cgi[0]))
		error(this->_socket, "close", "failed to close pipe");
	if (cgi_response == "")
		error(this->_socket, "CGI", "empty response");
	return (cgi_response);
}
