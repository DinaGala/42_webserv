#include "Cluster.hpp"
#include "Server.hpp"

int main(){

	Cluster clusterC;

	clusterC.setUpCluster();
	clusterC.runCluster();

	return (0);
}


