#include "Cluster.hpp"

Cluster::Cluster() {	
}

Cluster::~Cluster() {
}

void	Cluster::setUpCluster(){
    this->_nServers = 1;
	for (unsigned int i=0; i < _nServers; i++) {
        Server server;
		server.setUpServer();
		this->_servers.push_back(server);
    }
}

void	Cluster::runCluster(){
    Server server = this->_servers[0];
	while (1){
		/*ACCEPT
		int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
		addrlen is now a value-result argument. 
		It expects a pointer to an int that will be the size of addr. 
		After the function is executed, the int refered by addrlen will be set to the size of the peer address.
		*/
		// Grab a connection from the queue

		struct sockaddr_in sockAddress = server.getSockaddr();
		long unsigned int addrlen = sizeof(server.getSockaddr());
		int connection = accept(server.getSockfd(), (struct sockaddr*)&sockAddress, (socklen_t*)&addrlen);
		if (connection < 0) {
			std::cout << "Failed to grab connection. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string response = "HTTP/1.1 200 OK\n"
                           "Content-Type: text/html\n"
                           "Content-Length: 38\n\n"
                           "<html><body>This is my response</body></html>";
		// Read from the connection
		char buffer[1000];
		int bytesRead = recv(connection, buffer, 100, 0);
		
		std::cout << "Request: " << buffer;

		////////////////////////////
		int cgi = server.executeCgi();
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
		//////////////////////////
		/*SEND 
		send a message to the connection
		int send(int sockfd, const void *msg, int len, int flags); 
		*/
		//send(connection, response.c_str(), response.size(), 0);
    
		// Close the connections
		close(connection);
		
	}
	close(server.getSockfd());
}

/*	/*ACCEPT
	int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
	addrlen is now a value-result argument. 
	It expects a pointer to an int that will be the size of addr. 
	After the function is executed, the int refered by addrlen will be set to the size of the peer address.
	*/

	// Grab a connection from the queue
	/*auto addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
    	std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    	exit(EXIT_FAILURE);
	}*/

    // Read from the connection
	/*char buffer[100];
	auto bytesRead = read(connection, buffer, 100);
	std::cout << "Request: " << buffer;*/




	/*SEND 
	send a message to the connection
	int send(int sockfd, const void *msg, int len, int flags); 
	*/
	/*std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);*/
		
/*
	// Close the connections
	close(connection);
	close(sockfd);
	return 0;*/
