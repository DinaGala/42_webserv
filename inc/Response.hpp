#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Server.hpp"
# include "Request.hpp"
# include <iostream>
# include <map>
# include <vector>

class Server;
class Request;

class Response {
	private:
		std::vector<std::string>			_statusLine;
		std::map<std::string, std::string>	_headers;
		int									_errorCode;
		//body

	public:
		Response();
        Response(Request request);
		~Response();
};

#endif