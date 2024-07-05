#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <cstdlib> // For exit() and EXIT_FAILURE
# include <iostream> // For cout
# include <unistd.h> // For read
# include <cerrno>
# include <cstring>
# include <sys/types.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include "Cgi.hpp"

class Server {
	private:
		long					_sockfd;
		const char*				_ipAddress;
		int						_port;
		struct sockaddr_in		_sockaddr;

	public:
		Server();
		~Server();
		//ADD CANONICAL FORM

		void		setUpServer();
		void		initSocket();
		void		bindSocket();
		void		listenConnection();

		void		setIpAddress(const char* ipAddress);
		void		setPort(const int port);

		long				getSockfd() const;
		struct sockaddr_in	getSockaddr() const;
};

#endif
