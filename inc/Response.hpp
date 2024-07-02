#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>

class	Response
{
	public:
		Response();
		Response(const Response &r);
		~Response();
		Response	&operator=(const Response &r);
		std::string	&getResponse(const std::string &code);
		void		setBody(const std::string &msg);
		void		setCgi(const std::string &cgi);
		void		setCode(const int &code);
		std::string	putStatusLine(const std::string &code);
		void		putGeneralHeaders(void);
		bool		fileToBody(const std::string &path);
		void		sendError(const std::string &code, const std::string &path);
	private:
		static std::map<std::string, std::string>	_status;
		static std::map<std::string, std::string>	initStatus();
		std::string		_body;
		std::string		_response;
		std::string		_cgi;
		std::string		_path; //tmp
		std::string		_servname; //tmp
		std::string		_method; //tmp
		int				_timeout; //tmp
		int				_maxconnect; //tmp
		bool			_connection; //tmp
		unsigned int	_code;

		std::string	_itoa(std::string::size_type n);
};

#endif
