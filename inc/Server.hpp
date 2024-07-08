#ifndef SERVER_HPP
# define SERVER_HPP

# include "Cluster.hpp"

class Server {
	
	private:
	
	public:
		Server();
		~Server();

		void		setUpServer();
		void		initSocket();
		void		bindSocket();
		void		listenConnection();

		void		setIpAddress(const char* ipAddress);
		void		setPort(const int port);

};

#endif
