
#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Utils.hpp"

class Socket {
	private:
		Server&				_server;		
		int					_port;
		std::string			_ipAddress;
		long				_fd;
		bool				_master; // true if it's a master, false if a client
	//	struct sockaddr_in	_sockaddr;
		int					_nClients; //TODO: de 0 a MAX_CON
		Request&			_req;
		Response&			_resp;

	
	public:
		Socket(Server& server, int port);
		~Socket();

		void	setUpSocket();
		void	initSocket();
		void	bindSocket();
		void	listenConnection();

		void	setIpAddress(const std::string& ipAddress);
		void	setPort(const int port);

		const Server&				getServer() const;
		int							getPort() const;
		const std::string& 			getIpAdress() const;
		long						getSockfd() const;
//		const struct sockaddr_in&	getSockaddr() const;
};

#endif