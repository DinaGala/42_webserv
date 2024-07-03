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

//It needs:
// port, server, method, host
//Would be nice to have:
// content-type, content-length

class	Cgi
{
	public:
		Cgi(int port, const std::string &method);
		~Cgi();
		void	addPair(const std::string &ext, const std::string &cmd);
		std::string		executeCgi(void);
		void	setUrl(const std::string &url);
		void	setHost(const std::string &host);
		void	setServName(const std::string &serv);

	private:
		std::map<std::string, std::string>	_pairs;
		std::map<std::string, std::string>	_env;
		std::string							_url;
		std::string							_reqbody;

		Cgi();
		Cgi(const Cgi &c);
		Cgi		&operator=(const Cgi &c);
		char	**vecToMat(const std::vector<std::string> &vec);
		std::vector<std::string>	_parseUrl(const std::string &url);
		void	_searchFile(std::vector<std::string> vec);
		void	_setPathInfo(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
		void	_setQueryString(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end);
		char	**_getEnv(void);
		std::string	_childProcess(int *fdreq, int *fdcgi);
};

#endif
