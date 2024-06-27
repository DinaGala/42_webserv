#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Cluster.hpp"

class Server;

class Request {
	private:
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		int									_errorCode;
		//body

	public:
		Request();
        Request(const std::string& buffer);
		~Request();

		bool	parseRequest(const std::string & buffer);
		bool	parseRequestLine(const std::string & requestLineStr);
		bool	parseHeaders(const std::string & header);
		void 	fillRequestLineVector(const std::string & requestLineStr);
		std::vector<std::string>  createValidRequestVector();
};

#endif