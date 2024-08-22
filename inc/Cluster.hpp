#pragma once

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <ctime>
# include <sys/wait.h>
# include "Utils.hpp"
# include "Signals.hpp"
# include "Socket.hpp"
# include "Cgi.hpp"
# include "Parse.hpp"

class ServerConfig;
class Server;
class Socket;
class Cgi;

class Cluster {
	private:
		std::vector<ServerConfig>	_sconf; 
		std::vector<Server> 		_servers;
		std::list<Socket>			_sockets;
		int							_epFd; 
		int							_nfds; 
		struct epoll_event			_events[MAX_EVENTS]; 
		struct epoll_event			_ev; 
		
		std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >			_pids; //all active pids paired with: (1)(starting time, fd to read), (2) its Socket
		std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >::iterator findPidFromSocket(Socket *sock);
		void																		_checkChilds(void);//checks if there are child processes that are over

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
		void	eraseSocket(Socket *sock, bool err); // 0 - not an error, 1 - closing because of the error
		void	cleanSocket(Socket *sock);

};

std::ostream	&operator<<(std::ostream &out, const Socket &val);
std::ostream	&operator<<(std::ostream &out, const Request &val);
std::ostream	&operator<<(std::ostream &out, const Response &val);
std::ostream	&operator<<(std::ostream &out, const std::list<Socket> &val);

#endif
