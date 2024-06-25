#ifndef CLUSTER_HPP
# define CLUSTER_HPP
# include <vector>
# include "Server.hpp"
# include "Request.hpp"

class Server;
class Request;

class Cluster {
	private:
		std::vector<Server> _servers;
		std::vector<long>	_sockfd; //TODO
		Request				_request;
		int					_nServers;

	public:
		Cluster();
		~Cluster();

		void	setUpCluster();
		void 	runCluster();
};

#endif