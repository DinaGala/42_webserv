#pragma once

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "Utils.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Server.hpp"

class Request;
class Response;
class Server;

class Socket {
	private:
		Server&					_server;		
		int						_port;
		std::string				_ipAddress;
		int						_fd;
		bool					_master; // true if it's a master, false if a client
		std::vector<Request>	_req;
		std::vector<Response>	_resp;
		std::string				_response;
		Socket();
	
	public:
		Socket(Server& server, int port);
		Socket(Server &server, Socket *sock);
		~Socket();
		Socket& operator=(const Socket& src);
		Socket(const Socket& src);

		void	initMaster();
		void	initClient(int masterfd);
		void	setNonBlocking();
		void	cleanRequestResponse();

		/*	SETTERS	*/

		void	setIpAddress(const std::string& ipAddress);
		void	setPort(const int port);
		void	setResponse(std::string response);

		/*	GETTERS	*/

		Server&						getServer() const;
		int							getPort() const;
		const std::string& 			getIpAdress() const;
		int							getSockFd() const;
		bool						getMaster() const;
		Request*					getRequest() ;
		Response*					getResponse() ;
		std::string& 				getResponseLine() ;
};

#endif
