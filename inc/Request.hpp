#ifndef REQUEST_HPP
# define REQUEST_HPP

#define INITIAL_STATUS 0
#define REQUEST_LINE_PARSED 1
#define HEADERS_PARSED 2
#define FINISH_PARSED 3

# include "Cluster.hpp"

class Server;
class Socket;

class Request {
	private:
		std::string							_buffer;
		
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		int									_errorCode;

		std::string							_body;
		int									_status;	

		Socket								*socket;
		Server								*server;

	

	public:
        Request(const std::string & buffer);
		~Request();

		void	parseRequest();
		void	parseRequestLine();
		void	parseHeaders();
		void	parseBody();
		void 	createRequestLineVector(std::string requestLineStr);
		void	addHeaderToMap(std::string line);

		void	parseBodyByContentLenght();
		void	parseBodyByChunked();
		void	parseBodyByMultipartFormData();

		std::vector<std::string>	createValidRequestVector();
		bool 						isStringOfDigits(std::string line);
		uint64_t					convertStrToHex(std::string line);


};

#endif