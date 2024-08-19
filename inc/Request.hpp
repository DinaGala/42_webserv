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
		Server&								_server;
		int									_port;
		std::vector<std::string>			_requestLine;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;
		std::string							_path;
		std::string							_root;
		bool								_rootLoc;
		std::string							_method;
		int									_code;	
		int									_posLocation;									
		std::string 						_host;
		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::pair<std::string, std::string> >	_errorPages;
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
		std::vector<std::string> 			_cookiesEnv;

	public:
		Request(Server& server, int port);
		Request(const Request& src);
		Request& operator=(const Request& src);

		~Request();

		void		initParams();
		void		parseRequest(std::vector<unsigned char> buffer, int bytesRead);
		void		sendBadRequestError(std::string errMssg, int code);

		void		parseRequestLine();
		void		parseHeaders();
		void		parseBody();
		
		void 		createRequestLineVector(std::string requestLineStr);
		void		addHeaderToMap(std::string& line, std::map<std::string, std::string>& map);

		void		parseBodyByContentLength();
		void		parseBodyByChunked();
		int			findSizeChunk(size_t posEndSIze);
		uint64_t	convertStrToHex(std::string line);
		void		manageLineChunk(size_t posEndSIze, int sizeChunk);

		void		manageMultipartForm();
		void		getBoundary();
		void		saveMultipartHeaders();
		void		updateMultipartBody();
		void		saveFileName();

		void 		requestValidations();
		void		checkHost();
		void		manageAcceptedContent();
		void		checkConnectionKeepAlive();
		void 		managePath();
		void		checkQuery();
		void		checkLocation();
		void		updateInfoLocation();
		void		updateRoot();
		void 		updatePath();
		void		urlDecode();
		void		setCgi();
		void		setCookies();
		void 		checkAllowMethod();
		void		checkProtocolHttp();
		void		updateIndex();
		void		updateUploadDir();
		
		const std::string&								getBuffer() const;
		Server&											getServer() const;
		int												getPort() const;
		int												getStatus() const;
		const std::vector<std::string>&					getRequestLine() const;
		const std::map<std::string, std::string>&		getHeaders() const;
		const std::string&								getBody() const;
		const std::string&								getQuery() const;
		const std::string&								getPath() const;
		const std::string& 								getRoot() const;
		bool											getRootLoc() const;
		const std::string&								getMethod() const; 
		int												getCode() const;
		int												getPosLocation() const;
		const std::string& 								getHost() const;
		size_t											getMaxBodySize() const;
		const std::vector<std::string>& 				getAllowedMethods() const;
		const std::map<int, std::pair<std::string, std::string> >& getErrorPages() const;
		const std::string& 								getIndex() const;
		bool 											getAutoIndex() const;
		bool 											getAllowUpload() const;
		const std::string& 								getUploadDir() const;
		const std::string& 								getReturn() const;
		bool											getCgi() const; 
		const std::map<std::string, std::string>&		getCgiConf() const;
		const std::vector<std::string>&					getServerNames() const;
		bool											getConnectionKeepAlive() const;
		const std::multimap<std::string, std::string>&	getAcceptedContent() const;
		const std::string&								getBoundary() const;
		const std::map<std::string, std::string>&		getMultipartHeaders() const;
		const std::string& 								getFileName() const;
		const std::vector<std::string>&					getCookiesEnv() const;

		void 		setErrorPages(const std::map<int, std::pair<std::string, std::string> >&  errorPages);
		void 		setIndex(const std::string& index);
		void 		setAutoIndex(bool autoIndex);
		void 		setAllowUpload(bool allowUpload);
		void 		setUploadDir(const std::string& uploadDir);
		void 		setReturn(const std::string& alias);
		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);
		void 		setAllowMethod(const std::string& method);
};
#endif
