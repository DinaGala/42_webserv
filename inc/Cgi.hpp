#ifndef	CGI_HPP
# define CGI_HPP

#include <sys/wait.h>
#include <sys/stat.h>
#include <ctime>
#include "Request.hpp"

class	Request;

class	Cgi
{
	public:
		Cgi(const Request &rq);
		~Cgi();
		int		executeCgi(void);
		void	readCgi(std::string &resp);
		int		getCgiFd(void) const;
		int		getStatus(void) const;
		pid_t	getPid(void) const;

	private:
		int									_status;
		int									_cgi[2];
		pid_t								_pid;
		std::string							_url;
		std::string							_reqbody;
		std::vector<std::string>			_args;
		std::vector<std::string>			_cookiesEnv;
		std::map<std::string, std::string>	_env;

		void						_childProcess(int *fdreq);
		char**						_getEnv(void);
		char**						_vecToMat(const std::vector<std::string> &vec);
		std::vector<std::string>	_findArgs(const std::string &args,
										const std::map<std::string, std::string> &config) const;
};

#endif
