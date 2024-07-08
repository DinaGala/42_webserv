#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <iostream>
# include <map>
# include <vector>

# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <cstdlib> // For exit() and EXIT_FAILURE
# include <iostream> // For cout
# include <unistd.h> // For read
# include <cerrno>
# include <cstring>
# include <sys/types.h>
# include <arpa/inet.h>

#include <sstream>
#include <string>
#include <algorithm>

#include <stdexcept> // For standard exceptions

# include "Server.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Utils.hpp"

class Server;
class Socket;
class Request;
class Response;

class Cluster {
	private:
		std::vector<ServerConfig>	_sconf; 
		std::vector<Server> 		_servers;
		std::vector<Socket>			_sockets;
		int							_nServers; //TODO: pending configFIle
		int							_nSockets; //TODO: pending configFIle

	public:
		Cluster();
		~Cluster();

		void	setUpCluster(int ac, char **av);
		void 	runCluster();

		void	createServers();
		void	createSockets();

		int		acceptConnection(Socket socket);
};

#endif
