#ifndef CLUSTER_HPP
# define CLUSTER_HPP
# include <vector>
# include "Server.hpp"
#include "Response.hpp"

class Server;

class Cluster {
	private:
		std::vector<Server> _servers;
		int	_nServers;

	public:
		Cluster();
		~Cluster();


		void	setUpCluster();
		void 	runCluster();
};

#endif
