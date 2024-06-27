#include "Cluster.hpp"

Cluster::Cluster() {	
}

Cluster::~Cluster() {
}

void	Cluster::setUpCluster(){
	
	this->_nServers = 1; //TODO: pending configFIle
	this->_nSockets = 1; //TODO: pending configFIle

	createServers();
	createSockets();
}

void	Cluster::createServers(){
	for (unsigned int i=0; i < _nServers; i++) {
		Server server;
		server.setUpServer();
		this->_servers.push_back(server);
	}
}

void	Cluster::createSockets(){
	for (unsigned int i=0; i < _nSockets; i++) {
		Socket socket;
		socket.setUpSocket();
		this->_sockets.push_back(socket);
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
		std::cout << "Request: " << buffer;
		Request request;
		request.parseRequest(buffer);
		/*SEND 
			send a message to the connection
			int send(int sockfd, const void *msg, int len, int flags); 
		*/
		//Response response;
		//response.manageResponse(request);
		std::string response = "HTTP/1.1 200 OK\n"
							"Content-Type: text/html\n"
							"Content-Length: 38\n\n"
							"<html><body>Good talking to you!</body></html>";
		send(connection, response.c_str(), response.size(), 0);
			std::cerr << "hiii3 " << std::endl;

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