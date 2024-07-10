#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Utils.hpp"



class Socket {
	private:
		Server				&_server;		
		int					_port;
		std::string			_ipAddress;

		long				_sockfd;
		struct sockaddr_in	_sockaddr;
		std::vector<int>	_nClients; //TODO: pending multiplexing

	
	public:
		Socket(Server &server, int port);
		~Socket();

		void	setUpSocket();
		void	initSocket();
		void	bindSocket();
		void	listenConnection();

		void	setIpAddress(const std::string & ipAddress);
		void	setPort(const int port);


		long				getSockfd() const;
		struct sockaddr_in	getSockaddr() const;
		Server 			&getServer() const;
		const std::string 				getIp() const;
		int			getPort() const;

};

#endif