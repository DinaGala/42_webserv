#include "Cgi.hpp"

Cgi::Cgi(const Request &rq)
{
	this->_env["SERVER_SOFTWARE"] = "webserver";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_NAME"] = "server"; // getServerName()
	this->_env["REQUEST_METHOD"] = rq.getMethod();
	this->_env["PATH_INFO"]="/";
	this->_env["SCRIPT_NAME"] = rq.getPath();
	this->_env["QUERY_STRING"] = rq.getQuery();
	this->_args = this->_findArgs(rq.getPath(), rq.getCgiConf());
	this->_cookiesEnv = rq.getCookiesEnv(); //ADDED BY JULIA
}

Cgi::~Cgi() {}

/////////////////////// TRANSFORMATIONS FROM C++ TO C //////////////////////////

//from map<std::string, std::string> to char **
char	**Cgi::_getEnv(void)
{
	char	**mat;
	int		i = 0;
	std::string	tmp;
	std::map<std::string, std::string>::iterator end = this->_env.end();
	std::vector<std::string>::iterator	cookies_end = this->_cookiesEnv.end();

	mat = new char *[this->_env.size() + this->_cookiesEnv.size() + 1];
	if (!mat)
		throw std::bad_alloc();
	for (std::map<std::string, std::string>::iterator it = this->_env.begin();
			it != end; it++)
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
		i++;
	}
	/*ADDED BY JULIA*/
	for (std::vector<std::string>::iterator it = this->_cookiesEnv.begin(); it != cookies_end; it++)
  {
		mat[i] = strdup((*it).c_str());
		if (!mat[i])
		{
			for (int j = 0; j < i; ++j)
                delete[] mat[j];
        delete[] mat;
				throw std::bad_alloc();
		}
		i++;
	}	//FINISH
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

std::vector<std::string>	Cgi::_findArgs(const std::string &path, const std::map<std::string, std::string> &config) const
{
	std::vector<std::string>	args;
	std::string					ext;
	std::string::size_type		found;

	if (access(path.c_str(), X_OK) == 0) //if executable
		return (args.push_back("./" + path), args);
	found = path.find_last_of(".");
	if (found != std::string::npos)// if there's extension
		ext = path.substr(found);
	if (config.find(ext) != config.end()) //if it's not allowed
		args.push_back(config.at(ext));
	args.push_back(path);
	return (args);
}

/////////////////////////////// EXECUTION //////////////////////////////////////

void	Cgi::_childProcess(int *req)
{
	char	**args;
	char	**env;

	if (this->_args.size() == 0)
		exit(40);
	args = this->_vecToMat(this->_args);
	env = this->_getEnv();
	if (close(req[1]) || close(this->_cgi[0]))
		exit(50);
	if (dup2(req[0], STDIN_FILENO) == -1 || dup2(this->_cgi[1], STDOUT_FILENO) == -1)
		exit(50);
	if (close(req[0]) || close(this->_cgi[1]))
		exit(50);
	//for (size_t i = 0; i < this->_args.size(); i++)
	//	std::cerr << "ARGS: " << args[i] << std::endl;
	//std::cout << " ";
	//std::cout << "child process\n";
	execve(args[0], args, env);
	std::cout << "child process post exit\n";
	exit(50);
}

int	Cgi::executeCgi(void)
{
	//int		status;
	int		req[2];
	//std::time_t	epoch = std::time(NULL);

	if (pipe(req) || pipe(this->_cgi))
		return (500);
	fcntl(req[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(req[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->_cgi[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->_cgi[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);

	write(req[1], this->_reqbody.c_str(), this->_reqbody.size());
	write(req[1], "\0", 1);
	this->_pid = fork();
	if (this->_pid == -1)
		return (500);
	if (this->_pid == 0)
		this->_childProcess(req);
	if (close(req[0]) || close(this->_cgi[1]) || close(req[1]))
		return (500);
	/*std::cerr << "pre-while" << std::endl;
	while (1)
	{
		std::time_t now = std::time(NULL);
		if (now - epoch > timeout)
		{
			if (kill(this->_pid, SIGKILL))
			{
				std::cout << "I've killed the child!" << std::endl;
				waitpid(this->_pid, &status, 0);// wait until the child is actually dead
				return (500);
			}
			return (504);
		}
		if (waitpid(this->_pid, &status, WNOHANG) == -1)
		{
			std::cout << "FUCK, waitpid returns -1" << std::endl;
			return (500);
		}
		if (WIFEXITED(status))
		{
			std::cerr << "it has wexited\n";
			break ;
		}
	}
	this->readCgi(cgi_response);*/
	/*std::cerr << "pre-read" << std::endl;
	while ((count = read(this->_cgi[0], buffer, sizeof(buffer) - 1)) != 0)
	{
		if (count == -1)
		{
			perror("read");
			return (500);
		}
		buffer[count] = '\0';
		cgi_response += buffer;
	}
	std::cerr << "pre-last close" << std::endl;
	if (close(this->_cgi[0]))
		return (500);*/
	//return (WEXITSTATUS(status) * 10);
	return (0);
}
//child will exit with the proper error code.
//values higher than 255 will be modified by exit.
//To avoid this child will exit with status / 10.

/////////////////// READ /////////////////////

void	Cgi::readCgi(std::string &resp)
{
	int 	count;
	char	buffer[1024];

	while ((count = read(this->_cgi[0], buffer, sizeof(buffer) - 1)) != 0)
	{
		if (count == -1)
			return (void)(this->_status = 500);
		buffer[count] = '\0';
		resp += buffer;
	}
	if (close(this->_cgi[0]))
		return (void)(this->_status = 500);
}

//////////////////// GETTER ////////////////////////

pid_t	Cgi::getPid(void) const
{
	return (this->_pid);
}

int		Cgi::getCgiFd(void) const
{
	return (this->_cgi[0]);
}

int		Cgi::getStatus(void) const
{
	return (this->_status);
}
