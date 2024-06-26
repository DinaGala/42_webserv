#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <map>
#include <string>

class	Response
{
	public:
		Response();
		Response(const Response &r);
		~Response();
		Response	&operator=(const Response &r);
		std::string	&getResponse(const std::string &code);
		void		setBody(const std::string &msg);
	private:
		static	std::map<std::string, std::string>	_status;
		std::string	_body;
		std::string	_response;

		std::string	_ft_itoa(unsigned int n);
};

#endif
