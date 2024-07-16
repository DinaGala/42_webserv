#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"

class Server {
	
	private:
		std::string 						_ipAddress;
		std::vector<int>					_ports;
		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::string> 			_errorPages;
		std::string 						_index;
		bool 								_autoIndex;
		bool 								_allowUpload;
		std::string 						_uploadDir;
		std::string 						_return;
		std::map<std::string, std::string> 	_cgiConf;
		std::vector<std::string> 			_serverNames;

	public:
		Server(ServerConfig sconfig);
		~Server();

		void		initParamsServer(ServerConfig sconfig);
		
		void		setIpAddress(const char* ipAddress);
		void		setPort(const int port);
		void 		setErrorPages(const std::map<int, std::string>&  errorPages);
		void 		setIndex(const std::string& index);
		void 		setAutoIndex(bool autoIndex);
		void 		setAllowUpload(bool allowUpload);
		void 		setUploadDir(const std::string& uploadDir);
		void 		setReturn(const std::string& alias);
		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);

		const std::string& 							getIpAdress() const;
		const std::vector<int>&						getPort() const;
		size_t 										getMaxBodySize() const;
		const std::vector<std::string>& 			getAllowedMethods() const;
		const std::map<int, std::string>& 			getErrorPages() const;
		const std::string& 							getIndex() const; //LOCATION
		bool 										getAutoIndex() const;
		bool 										getAllowUpload() const; //LOCATION
		const std::string& 							getUploadDir() const; //LOCATION
		const std::string& 							getReturn() const; //LOCATION
		const std::map<std::string, std::string>&	getCgiConf() const;
		const std::vector<std::string>& 			getServerNames() const;
};

#endif
