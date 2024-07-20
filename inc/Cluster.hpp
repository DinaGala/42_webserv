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
		int							_epFd; //epoll instance id
		int							_nfds; // number of actual events
		struct epoll_event			_events[MAX_EVENTS]; // an array used to collect the events that epoll detects.
		struct epoll_event			_ev; //is used to describe the events we are interested in for a particular file descriptor.

	public:
		Cluster();
		~Cluster();

		void	setUpCluster(int ac, char **av);
		void 	runCluster();

		void	createServers();
		void	createSockets();
		void 	createEpoll();

		void	acceptConnection(Socket *sock);
		void	readConnection(Socket *sock);
		void	sendConnection(Socket *sock);
		void	modifyEvent(Socket *sock, bool flag); // 0 - in, 1 - out
};

#endif
