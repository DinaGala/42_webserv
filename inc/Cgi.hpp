#ifndef	CGI_HPP
# define CGI_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctime>

//It needs:
// port, server, method, host
//Would be nice to have:
// content-type, content-length

class	Cgi
{
	public:
		Cgi(int port, const std::string &method, int socket);
		~Cgi();
		bool	executeCgi(std::string &response, int timeout);
		void	setEnvVars(const std::string &u, const std::string &h, const std::string &s);


	private:
		std::map<std::string, std::string>	_pairs;
		std::map<std::string, std::string>	_env;
		std::string							_url;
		std::string							_reqbody;
		int									_socket;

		Cgi();
		Cgi(const Cgi &c);
		Cgi		&operator=(const Cgi &c);
		void	_searchFile(std::vector<std::string> vec);
		void	_setPathInfo(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
		void	_setQueryString(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
		void	_childProcess(int *fdreq, int *fdcgi);
		char	**_getEnv(void);
		char	**_vecToMat(const std::vector<std::string> &vec);
		std::vector<std::string>	getArgs(void);
		std::vector<std::string>	_parseUrl(const std::string &url);
};

#endif
