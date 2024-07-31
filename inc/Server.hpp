#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"

class Server {
	
	private:
		std::string 						_ipAddress;
		std::string 						_host;
		std::vector<int>					_ports;
		std::string							_root;
		size_t 								_maxBodySize;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::pair<std::string, std::string> >	_errorPages;
		bool 								_autoIndex;
		std::string 						_return;
		std::map<std::string, std::string> 	_cgiConf;
		std::vector<std::string> 			_serverName;
		std::vector<LocationConfig> 		_locations;

	public:
		Server();
		Server(ServerConfig &sconfig);
		Server(const Server& src);
		Server& operator=(const Server& src);
		std::map<int, std::pair<std::string, std::string> >	operator=(const std::map<int, std::pair<std::string, std::string> > &val);
		~Server();

		void		initParamsServer(ServerConfig &sconfig);
		
		void		setIpAddress(const char* ipAddress);
		void		setPort(const int port);
		void 		setErrorPages(const std::map<int, std::pair<std::string, std::string> >&  errorPages);
		void 		setIndex(const std::string& index);
		void 		setAutoIndex(bool autoIndex);
		void 		setReturn(const std::string& alias);
		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);

		const std::string& 							getIpAdress() const;
		const std::string& 							getHost() const;
		const std::vector<int>&						getPort() const;
		const std::string& 							getRoot() const;
		size_t 										getMaxBodySize() const;
		const std::vector<std::string>& 			getAllowedMethods() const;
		const std::map<int, std::pair<std::string, std::string> >&	getErrorPages() const;
		bool 										getAutoIndex() const;
		const std::string& 							getReturn() const;
		const std::map<std::string, std::string>&	getCgiConf() const;
		const std::vector<std::string>& 			getServerName() const;
		const std::vector<LocationConfig> 			getLocationConfig() const;
};

#endif
