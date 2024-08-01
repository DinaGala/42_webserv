#include "Cgi.hpp"

Cgi::Cgi(const Request &rq, std::vector<std::string> &args): _status(0)
{
	this->_env["SERVER_SOFTWARE"] = "webserver";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = "server"; // getServerName()
	this->_env["REQUEST_METHOD"] = rq.getMethod();
	this->_env["PATH_INFO"]="/";
	this->_env["SCRIPT_NAME"] = rq.getPath();
	this->_env["QUERY_STRING"] = rq.getQuery();
	this->_args = args;
}

Cgi::~Cgi() {}

/////////////////////// TRANSFORMATIONS FROM C++ TO C //////////////////////////

//from map<std::string, std::string> to char **
char	**Cgi::_getEnv(void)
{
	char	**mat;
	int		i = 0;
	std::string	tmp;
	std::map<std::string, std::string>::iterator it = this->_env.begin();
	std::map<std::string, std::string>::iterator end = this->_env.end();

	mat = new char *[this->_env.size() + 1];
	if (!mat)
		throw std::bad_alloc();
	while (it != end)
	{
		tmp = it->first + "=" + it->second;
		mat[i] = strdup(tmp.c_str());
		if (!mat[i])
		{
			for (int j = 0; j < i; ++j)
                delete[] mat[j];
            delete[] mat;
				throw std::bad_alloc();
		}
		it++;
		i++;
	}
	mat[i] = NULL;
	return (mat);
}

//from vector<std::string> to char **
char	**Cgi::_vecToMat(const std::vector<std::string> &vec)
{
	char	**mat;
	size_t	len = vec.size();

	mat = new char *[len + 1];
	if (!mat)
		throw std::bad_alloc();
	mat[len] = NULL;
	for (size_t i = 0; i < len; i++)
	{
		mat[i] = new char [vec[i].size() + 1];
		if (!mat[i])
			throw std::bad_alloc();
		std::strcpy(mat[i], vec[i].c_str());
		mat[i][vec[i].size()] = '\0';
		if (!mat[i])
		{
			for (size_t j = 0; j < i; ++j)
                delete[] mat[j];
            delete[] mat;
			throw std::bad_alloc();
		}
	}
	return (mat);
}

////////////////////////////////////////////////////////////////////////////////


///////////////////////////// PARSING SETTERS///////////////////////////////////

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

/////////////////////////////// EXECUTION //////////////////////////////////////

void	Cgi::_childProcess(int *req, int *cgi)
{
	char	**args;
	char	**env;
	std::cerr << "\033[1;31mchildProcess args.size() " << this->_args.size()<< "\033[0m" << std::endl;
	if (this->_args.size() == 0)
		exit(40);
	args = this->_vecToMat(this->_args);
	env = this->_getEnv();
	if (close(req[1]) || close(cgi[0]))
		exit(50);
	if (dup2(req[0], STDIN_FILENO) == -1 || dup2(cgi[1], STDOUT_FILENO) == -1)
		exit(50);
	if (close(req[0]) || close(cgi[1]))
		exit(50);
	execve(args[0], args, env);
	exit(50);
}

int	Cgi::executeCgi(std::string &cgi_response, int timeout)
{
	pid_t	pid;
	int		status;
	int		req[2], cgi[2];
	char buffer[1024];
	ssize_t count;
	std::time_t	epoch = std::time(NULL);

	if (pipe(req) || pipe(cgi))
		return (500);
	write(req[1], this->_reqbody.c_str(), this->_reqbody.size());
	write(req[1], "\0", 1);
	pid = fork();
	if (pid == -1)
		return (500);
	if (pid == 0)
		this->_childProcess(req, cgi);
	if (close(req[0]) || close(cgi[1]) || close(req[1]))
		return (500);
	while (1)
	{
		std::time_t now = std::time(NULL);
		if (now - epoch > timeout)
		{
			kill(pid, 0);
			return (504);
		}
		if (waitpid(pid, &status, WNOHANG) == -1)
			return (500);
		if (WIFEXITED(status))
			break ;
	}
	while ((count = read(cgi[0], buffer, sizeof(buffer) - 1)) != 0)
	{
		if (count == -1)
			return (500);
		buffer[count] = '\0';
		cgi_response += buffer;
	}
	if (close(cgi[0]))
		return (500);
	return (WEXITSTATUS(status) * 10);
}
//child will exit with the proper error code.
//values higher than 255 will be modified by exit.
//To avoid this child will exit with status / 10.
