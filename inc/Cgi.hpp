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

//It needs:
// port, server, method, host
//Would be nice to have:
// content-type, content-length

class	Cgi
{
	public:
		Cgi(const std::string &serv, int port, const std::string &method, const std::string &host);
		~Cgi();
		void	addPair(const std::string &ext, const std::string &cmd);
		char	**getEnv(void);
		int		executeCgi(void);
		void	setContentLength(const std::string &clen);
		void	setContentType(const std::string &ctype);
		//void	setUrlEnv(const std::string &url);
		void	parseUrl(const std::string &url);

	private:
		std::map<std::string, std::string>	_pairs;
		std::map<std::string, std::string>	_env;

		Cgi();
		Cgi(const Cgi &c);
		Cgi		&operator=(const Cgi &c);
		char	**vecToMat(const std::vector<std::string> &vec);
};

#endif
