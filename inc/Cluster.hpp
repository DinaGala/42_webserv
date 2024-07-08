#ifndef CLUSTER_HPP
# define CLUSTER_HPP

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
