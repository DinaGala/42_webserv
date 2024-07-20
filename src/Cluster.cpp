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
	createEpoll();
}

void	Cluster::createServers(){
	for (std::vector<ServerConfig>::iterator it = _sconf.begin(); it != _sconf.end(); it++) {
		Server server(*it);
		this->_servers.push_back(server);
	}
}

void	Cluster::createSockets(){
	for (size_t i = 0; i < _servers.size(); i++) {
		std::vector<int> ports = _servers[i].getPort();
		for (size_t j = 0; j < ports.size(); j++) {
			Socket socket(_servers[i], ports[j]);
			this->_sockets.push_back(socket);
		}
	}
}

void	Cluster::createEpoll()
{
	std::string errmsg;
	
	_epFd = epoll_create(10);
	if (_epFd == -1)
		throw std::runtime_error("Error: epoll create failed: " + errmsg.assign(strerror(errno)));
	
	_ev.events = EPOLLIN;
	for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); it++) 
	{
		_ev.data.fd = it->getSockFd();
		_ev.data.ptr = &(*it);
		if (epoll_ctl(_epFd, EPOLL_CTL_ADD, it->getSockFd(), &_ev) == -1) 
			throw std::runtime_error("Error: epoll control failed: " + errmsg.assign(strerror(errno)));
	}
}


void	Cluster::runCluster(){
    // Server server = _servers[0]; //TODO: find server
	// Socket socket = _sockets[0]; //TODO: identify socket
	std::string errmsg;


	while (1) //manage signals
	{ 
		_nfds = epoll_wait(_epFd, _events, MAX_EVENTS, -1);
        if (_nfds == -1)
			throw std::runtime_error("Error: epoll wait failed: " + errmsg.assign(strerror(errno)));
		for (int n = 0; n < _nfds; ++n)
		{
			Socket *cur = static_cast<Socket *>(_events[n].data.ptr);
			if (_events[n].events & EPOLLIN)
			{
				if (cur->getMaster())
					//EPOLLIN
					acceptConnection(cur);
				else
					//EPOLLIN, if all read - change to EPOLLOUT 
					readConnection(cur);
			}
			else if (_events[n].events & EPOLLOUT)
			// send in chunks and change to epollin when finished. New request? OR close? 
				sendConnection(cur);
			else //ADD timeout
				throw std::runtime_error("Error: epoll event error ");
		}


	}
}


	/*ACCEPT
	**int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
	**addrlen is now a value-result argument. 
	**It expects a pointer to an int that will be the size of addr. 
	**After the function is executed, the int refered by addrlen will be set to the size of the peer address.
	*/
	// Grab a connection from the queue
void	Cluster::acceptConnection(Socket *sock)
{
	std::string errmsg;
	Socket socket(sock->getServer(), sock);
	
	_sockets.push_back(socket);

	_ev.events = EPOLLIN;
	_ev.data.fd = socket.getSockFd();
	_ev.data.ptr = &socket;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, socket.getSockFd(), &_ev) == -1) 
		throw std::runtime_error("Error: epoll control failed: " + errmsg.assign(strerror(errno)));


}

void	Cluster::readConnection(Socket *sock)
{
		if (sock->getRequest()->getStatus() == FINISH_PARSED)
			return (modifyEvent(sock, 1));

		char buffer[BUFFER_SIZE + 1];
		int bytesRead = read(sock->getSockFd(), buffer, BUFFER_SIZE);
		if (bytesRead <= 0)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client.fd, nullptr) == -1) {
                                perror("epoll_ctl: EPOLL_CTL_DEL");
                            }
            close(sock->getSockFd);
            _sockets.erase(*sock);
		}

		buffer[bytesRead] = '\0';
		sock->getRequest()->parseRequest(buffer);
		
		if (sock->getRequest()->getStatus() == FINISH_PARSED)
			return (modifyEvent(sock, 1));

		sock->setResponse(sock->getResponse()->makeResponse(sock->getRequest()));


}

void	Cluster::sendConnection(Socket *sock)
{

		/*SEND 
			send a message to the connection
			int send(int sockfd, const void *msg, int len, int flags); 
		*/
		send(connection, response.c_str(), response.size(), 0);

		// Close the connections
		close(connection);

	close(socket.getSockfd());
}

// if flag 0 - to in, 1 - to out
void	Cluster::modifyEvent(Socket *sock, bool flag)
{
	if (flag == 1)
	{
		_ev.events = EPOLLOUT;
		_ev.data.fd = sock->getSockFd();
		_ev.data.ptr = sock;
		epoll_ctl(_epFd, EPOLL_CTL_MOD, sock->getSockFd(), &_ev);
	}
	else
	{
		_ev.events = EPOLLIN;
		_ev.data.fd = sock->getSockFd();
		_ev.data.ptr = sock;
		epoll_ctl(_epFd, EPOLL_CTL_MOD, sock->getSockFd(), &_ev);
	}
}