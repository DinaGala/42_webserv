#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Cluster.hpp"

class Socket {
	private:
		long				_sockfd;
		const char*			_ipAddress;
		int					_port;
		struct sockaddr_in	_sockaddr;
		std::vector<int>	_nClients; //TODO: pending multiplexing
	
	public:
		Socket();
		~Socket();

		void	setUpSocket();
		void	initSocket();
		void	bindSocket();
		void	listenConnection();

		void	setIpAddress(const char* ipAddress);
		void	setPort(const int port);


		long				getSockfd() const;
		struct sockaddr_in	getSockaddr() const;

};

#endif