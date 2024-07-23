#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

# define INITIAL_STATUS 0
# define REQUEST_LINE_PARSED 1
# define HEADERS_PARSED 2
# define BODY_PARSED 3
# define FINISH_PARSED 4
# define CRLF "\r\n"


# include "Utils.hpp"

class Server;

class Request {
	private:
		std::string							_buffer;
		int									_status;
		std::vector<Server*>		    		_serv;
		//Server*								_server;
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;
		std::string							_path;
		std::string							_root;
		int									_code;
	
		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::string> 			_errorPages;
		std::string 						_index;
		bool 								_autoIndex;
		bool 								_allowUpload;
		std::string 						_uploadDir;
		std::string 						_return;
		bool								_cgi;
		std::map<std::string, std::string> 	_cgiConf;

		std::vector<std::string> 			_serverNames; 
		bool								_connectionKeepAlive;
		std::multimap<std::string, std::string>	_acceptedContent;

		std::string							_boundary;
		std::map<std::string, std::string>	_multipartHeaders;
		std::string							_fileName;
		bool								_location;


	public:
		Request();
		Request(Server* server);
		//Request(const Request& src);
		//Request& operator=(const Request& src);
		~Request();
		void	initParamsRequest();
		void	parseRequest(const std::string& buffer);
		void	sendBadRequestError(std::string errMssg);

		void	parseRequestLine();
		void	parseHeaders();
		void	parseBody();
		
		void 	createRequestLineVector(std::string requestLineStr);
		void	addHeaderToMap(std::string& line, std::map<std::string, std::string>& map);
		void	checkConnectionKeepAlive();
		
		void		parseBodyByContentLength();
		void		parseBodyByChunked();
		int			findSizeChunk(size_t posEndSIze);
		uint64_t	convertStrToHex(std::string line);
		void		manageLineChunk(size_t posEndSIze, int sizeChunk);
		void		checkAcceptedContent();

		void		manageMultipartForm();
		void		getBoundary();
		void		saveMultipartHeaders();
		void		updateMultipartBody();
		void		saveFileName();

		void		cleanRequest(); // TODO for multiplexing
		void 		requestValidations();
		void 		checkPath();
		std::string checkQuery();
		size_t		checkLocation(std::string & path);
		void		updateInfoLocation(size_t nLoc);
		void 		checkAllowMethod();
		void		checkProtocolHttp();
		void 		updatePath();
		
		const std::string&							getBuffer() const;
		//Server*										getServer() const;
		int											getStatus() const;
		const std::vector<std::string>&				getRequesLine() const;
		const std::map<std::string, std::string>&	getHeaders() const;
		const std::string&							getBody() const;
		const std::string&							getQuery() const;
		const std::string&							getPath() const;
		const std::string& 							getRoot() const;
		int											getCode() const;
		size_t										getMaxBodySize() const;
		const std::vector<std::string>& 			getAllowedMethods() const;
		const std::map<int, std::string>& 			getErrorPages() const;
		const std::string& 							getIndex() const;
		bool 										getAutoIndex() const;
		bool 											getAllowUpload() const;
		const std::string& 								getUploadDir() const;
		const std::string& 								getReturn() const;
		bool											getCgi() const; 
		const std::map<std::string, std::string>&		getCgiConf() const;
		const std::string&								getMethod() const;
		const std::vector<std::string>&					getServerNames() const;
		bool											getConnectionKeepAlive() const;
		const std::multimap<std::string, std::string>&	getAcceptedContent() const;
		const std::string&								getBoundary() const;
		const std::map<std::string, std::string>&		getMultipartHeaders() const;
		const std::string& 								getFileName() const;
		bool											getLocation() const;

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
