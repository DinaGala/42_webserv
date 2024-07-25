
# include "Cluster.hpp"
#include "Utils.hpp"

int main(int ac, char **av){
	
	try {
		if (ac > 2)
			throw std::invalid_argument("Wrong amount of arguments: introduce only one config file or nothing");
		Cluster clusterC;

		clusterC.setUpCluster(ac, av);
		//clusterC.runCluster();
	}
	catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
	}
	return (0);
}

