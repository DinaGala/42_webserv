#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Cluster.hpp"

class Server;

class Socket {
	private:
		long				_sockfd;
		std::string			_ipAddress;
		int					_port;
		struct sockaddr_in	_sockaddr;
		std::vector<int>	_nClients; //TODO: pending multiplexing
		Server				*_server;
	
	public:
		Socket(Server server, int port);
		~Socket();

		void	setUpSocket();
		void	initSocket();
		void	bindSocket();
		void	listenConnection();

		void	setIpAddress(const std::string & ipAddress);
		void	setPort(const int port);


		long				getSockfd() const;
		struct sockaddr_in	getSockaddr() const;
		Server				*getServer() const;

};

#endif