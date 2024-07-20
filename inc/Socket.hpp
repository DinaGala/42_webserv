
#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Utils.hpp"

class Socket {
	private:
		Server&				_server;		
		int					_port;
		std::string			_ipAddress;
		int					_fd;
		bool				_master; // true if it's a master, false if a client
	//	struct sockaddr_in	_sockaddr;
		int					_nClients; //TODO: de 0 a MAX_CON
		Request				_req;
		Response			_resp;
		std::string			_response;
		Socket();

	
	public:
		Socket(Server& server, int port);
		Socket(Server &server, Socket *sock);
		~Socket();
		Socket& operator=(const Socket& src);
		Socket(const Socket& src);
	//	Socket();

	//	void	setUpSocket();
		void	initMaster();
		void	initClient(int masterfd);
		void	setNonBlocking();

	//	void	bindSocket();
	//	void	listenConnection();

		void	setIpAddress(const std::string& ipAddress);
		void	setPort(const int port);
		void	setResponse(std::string response);

		Server&						getServer() const;
		const int					getPort() const;
		const std::string& 			getIpAdress() const;
		const int					getSockFd() const;
		bool						getMaster() const;
		Request*				getRequest() ;
		Response*				getResponse() ;
//		const struct sockaddr_in&	getSockaddr() const;
};

#endif
