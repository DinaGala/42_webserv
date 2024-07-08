#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"

class Server {
	
	private:
		std::string 				_ipAddress;
		std::vector<int>			_ports;
		std::vector<std::string> 	_allowedMethods;
		size_t 						_maxBodySize; 

	public:
		Server(ServerConfig sconfig);
		~Server();

		void		initSocket();
		void		bindSocket();
		void		listenConnection();

		void		setIpAddress(const char* ipAddress);
		void		setPort(const int port);

		const std::string 				getIp() const;
		const std::vector<int>			getPort() const;
		size_t 							getMaxBodySize() const;
		const std::vector<std::string> 	getAllowedMethods() const;
};

#endif
