#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <map>
#include <string>
#include <ctime>

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
		void		putStatusLine(const std::string &code);
		void		putGeneralHeaders(void);
	private:
		static const std::map<std::string, std::string>	_status;
		static std::map<std::string, std::string>	initStatus();
		std::string		_body;
		std::string		_response;
		std::string		_cgi;
		std::string		_servname;
		unsigned int	_code;


		std::string	_itoa(std::string::size_type n);
};

#endif
