#ifndef REQUEST_HPP
# define REQUEST_HPP

#define INITIAL_STATUS 0
#define REQUEST_LINE_PARSED 1
#define HEADERS_PARSED 2
#define BODY_PARSED 3
#define FINISH_PARSED 4

# include "Utils.hpp"

class Request {
	private:
		std::string							_buffer;
		int									_status;
		Socket&								_socket;
		
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;
		std::string							_path;
		int									_code;
	
		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::string> 			_errorPages;
		std::string 						_index;
		bool 								_autoIndex;
		bool 								_allowUpload;
		std::string 						_uploadDir;
		std::string 						_return; //TODO: is it necessary to add boolean?
		bool								_cgi;
		std::map<std::string, std::string> 	_cgiConf;
		std::vector<std::string> 			_serverNames; 
		bool								_connectionKeepAlive;
		std::multimap<std::string, std::string>	_acceptedContent;

		std::string							_boundary;
		std::map<std::string, std::string>	_multipartHeaders;
		std::string							_fileName;


	public:
        Request(Socket& socket);
		~Request();
		void	initParamsRequest();
		void	parseRequest(const std::string& buffer);
		
		void	parseRequestLine();
		void	parseHeaders();
		void	parseBody();
		
		void 	createRequestLineVector(std::string requestLineStr);
		void 	checkPath();
		void	checkProtocolHttp();
		void 	checkAllowMethod();
		void	addHeaderToMap(std::string& line, std::map<std::string, std::string>& map);
		void	checkConnectionKeepAlive();
		
		void	parseBodyByContentLength();
		void	parseBodyByChunked();
		void	checkAcceptedContent();

		void	manageMultipartForm();
		void	getBoundary();
		void	saveMultipartHeaders();
		void	updateMultipartBody();
		void	saveFileName();

		bool 						isStringOfDigits(std::string line);
		uint64_t					convertStrToHex(std::string line);
		
		const Socket&								getSocket() const;
		const std::map<std::string, std::string>&	getHeaders() const;
		const std::string&							getBody() const;
		const std::string&							getQuery() const;
		const std::string&							getPath() const;
		int											getCode() const;
		size_t										getMaxBodySize() const;
		const std::vector<std::string>& 			getAllowedMethods() const;
		const std::map<int, std::string>& 			getErrorPages() const;
		const std::string& 							getIndex() const; //LOCATION
		bool 										getAutoIndex() const;

		bool 										getAllowUpload() const; //LOCATION
		const std::string& 							getUploadDir() const; //LOCATION
		const std::string& 							getReturn() const; //LOCATION
		bool										getCgi() const; 
		const std::map<std::string, std::string>&	getCgiConf() const;
		const std::string&							getMethod() const;
		const std::vector<std::string>&				getServerNames() const; //LOCATION
		bool										getConnectionKeepAlive() const;
		const std::multimap<std::string, std::string>&	getAcceptedContent() const;
		const std::map<std::string, std::string>&	getMultipartHeaders() const;
		const std::string& 							getFileName() const;

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