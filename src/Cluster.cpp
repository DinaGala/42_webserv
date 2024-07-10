#include "Cluster.hpp"

Cluster::Cluster() {	
}

Cluster::~Cluster() {
}

void	Cluster::setUpCluster(int ac, char **av){
	if (ac == 2) {
        _sconf = Parse::configParse(av[1]);
			std::cout << _sconf;
	}
	else if (ac == 1)
	{
        _sconf = Parse::configParse();
			std::cout << _sconf;
	}	
	
	createServers();
	createSockets();
	std::cout << "----Num servers " << _servers.size();
	std::cout << " | Num sockets " << _sockets.size() << "----" << std::endl;
}

void	Cluster::createServers(){
	for (std::vector<ServerConfig>::iterator it = _sconf.begin(); it != _sconf.end(); it++) {
		Server server(*it);
		this->_servers.push_back(server);
	}
}

void	Cluster::createSockets(){
	for (std::vector<Server>::iterator itServers = _servers.begin(); itServers != _servers.end(); itServers++) {
		Server server = (Server)*itServers;
		std::vector<int> ports = server.getPort();
		for (std::vector<int>::iterator itPorts = ports.begin(); itPorts != ports.end(); itPorts++) {
			Socket socket(*itServers, *itPorts);
			socket.setUpSocket();
			this->_sockets.push_back(socket);
		}
	}
}


void	Cluster::runCluster(){
    Server server = _servers[0]; //TODO: find server
	Socket socket = _sockets[0]; //TODO: identify socket

	while (1){ //manage signals
		int	connection = acceptConnection(socket);

		// Read from the connection
		
		char buffer[1000];
		int bytesRead = read(connection, buffer, 100);
		if (bytesRead < 0)
			return; //TODO: manage error
		Request request(buffer, &socket); //Add server and socket
		request.parseRequest();
	 
		//request.manageRequest(); 

		/////////////////////////////
		///////// RESPONSE /////////
		///////////////////////////
		Response	rsp;
		rsp.setCgiPath("a");
		rsp.setMethod("GET");
		rsp.setSocket((int)socket.getSockfd());
		std::string response = rsp.getResponse("200");
		std::cout << std::endl << "RESPONSE" << std::endl << response << std::endl;
		///////////////////////////
		///////////////////////////

		/*SEND 
			send a message to the connection
			int send(int sockfd, const void *msg, int len, int flags); 
		*/
		send(connection, response.c_str(), response.size(), 0);

		// Close the connections
		close(connection);
	}
	close(socket.getSockfd());
}

int	Cluster::acceptConnection(Socket socket){
	/*ACCEPT
	**int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
	**addrlen is now a value-result argument. 
	**It expects a pointer to an int that will be the size of addr. 
	**After the function is executed, the int refered by addrlen will be set to the size of the peer address.
	*/
	// Grab a connection from the queue

	struct sockaddr_in sockAddress = socket.getSockaddr();
	long unsigned int addrlen = sizeof(socket.getSockaddr());
	int connection = accept(socket.getSockfd(), (struct sockaddr*)&sockAddress, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cerr << "Failed to grab connection. errno: " << std::endl;
		exit(EXIT_FAILURE);
	}
	return connection;
}