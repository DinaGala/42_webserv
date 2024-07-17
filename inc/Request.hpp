#ifndef REQUEST_HPP
# define REQUEST_HPP

#define INITIAL_STATUS 0
#define REQUEST_LINE_PARSED 1
#define HEADERS_PARSED 2
#define FINISH_PARSED 3

# include "Utils.hpp"

class Request {
	private:
		std::string							_buffer;
		int									_status;	
		Socket&								_socket;
		
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		int									_errorCode;

		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::string> 			_errorPages;
		std::string 						_index;
		bool 								_autoIndex;
		bool 								_allowUpload;
		std::string 						_uploadDir;
		std::string 						_return; //TODO: is it necessary to add boolean?
		std::map<std::string, std::string> 	_cgiConf;
		
	public:
        Request(const std::string& buffer, Socket& socket);
		~Request();
		void	initParamsRequest();
		void	parseRequest();
		
		void	parseRequestLine();
		void	parseHeaders();
		void	parseBody();
		
		void 	createRequestLineVector(std::string requestLineStr);
		void 	checkUrlPath();
		void	checkProtocolHttp();
		void 	checkAllowMethod();
		void	addHeaderToMap(std::string line);
		
		void	parseBodyByContentLength();
		void	parseBodyByChunked();

		bool 						isStringOfDigits(std::string line);
		uint64_t					convertStrToHex(std::string line);
		
		const Socket&								getSocket() const;
		const std::vector<std::string>& 			getAllowedMethods() const;
		const std::map<int, std::string>& 			getErrorPages() const;
		const std::string& 							getIndex() const; //LOCATION
		bool 										getAutoIndex() const;
		bool 										getAllowUpload() const; //LOCATION
		const std::string& 							getUploadDir() const; //LOCATION
		const std::string& 							getReturn() const; //LOCATION
		const std::map<std::string, std::string>&	getCgiConf() const;

		
		void 		setErrorPages(const std::map<int, std::string>&  errorPages);
		void 		setIndex(const std::string& index);
		void 		setAutoIndex(bool autoIndex);
		void 		setAllowUpload(bool allowUpload);
		void 		setUploadDir(const std::string& uploadDir);
		void 		setReturn(const std::string& alias);
		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);
		void 		setAllowMethod(const std::string& method);
};
#endif