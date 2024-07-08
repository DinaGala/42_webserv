#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Cluster.hpp"

class Server;
class Request;

class Response {
	private:
		std::vector<std::string>			_statusLine;
		std::map<std::string, std::string>	_headers;
		int									_errorCode;
		//body


		static	std::map<std::string, std::string>	_status;
		std::string	_body;
		std::string	_response;

		std::string	_ft_itoa(unsigned int n);

	public:
		Response();
		Response(const Response &r);
		~Response();
		Response	&operator=(const Response &r);
		std::string	&getResponse(const std::string &code);
		void		setBody(const std::string &msg);

		void    manageResponse();
};

#endif
