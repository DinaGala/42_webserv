#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Cluster.hpp"

class Server;
class Socket;

class Request {
	private:
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		int									_errorCode;
		Socket								*socket;
		Server								*server;
		//body

	public:
		Request();
        Request(const std::string& buffer);
		~Request();

		void	parseRequest(const std::string & buffer);
		void	parseRequestLine(const std::string & requestLineStr);
		void	parseHeaders(const std::string & header);
		void 	fillRequestLineVector(std::string requestLineStr);
		std::vector<std::string>  createValidRequestVector();
};

#endif