
# include "Cluster.hpp"
#include "Utils.hpp"

int main(int ac, char **av){
	
	try {
		if (ac > 2)
			throw std::invalid_argument("Wrong amount of arguments: introduce only one config file or nothing");
		Cluster clusterC;

		clusterC.setUpCluster(ac, av);
		clusterC.runCluster();
	}
	catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
	}
	/********* TMP **************/
	int fd = open("Makefile", O_RDONLY);
	std::cout << "\033[1;32mIf this fd is not 3, we're leaking fd: " << fd << std::endl;
	close(fd);
	/*************************/
	return (0);
}

