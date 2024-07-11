#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Utils.hpp"

class Server;
class Socket;

class Cluster {
	private:
		std::vector<ServerConfig>	_sconf; 
		std::vector<Server> 		_servers;
		std::vector<Socket>			_sockets;

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
