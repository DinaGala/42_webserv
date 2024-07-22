#include "Cluster.hpp"
#include "Utils.hpp"

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


void	Cluster::runCluster()
{
	std::string errmsg;

	while (1) //manage signals
	{ 
		_nfds = epoll_wait(_epFd, _events, MAX_EVENTS, 2000); // check 2000
        if (_nfds == -1)
			throw std::runtime_error("Error: epoll wait failed: " + errmsg.assign(strerror(errno)));
		for (int n = 0; n < _nfds; ++n)
		{
			Socket *cur = static_cast<Socket *>(_events[n].data.ptr);
			std::cout << "current socket is:  " << cur << "\n";
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
		//checkTimeout();
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
		char buffer[BUFFER_SIZE + 1];
		int bytesRead = recv(sock->getSockFd(), buffer, BUFFER_SIZE, MSG_DONTWAIT);
		if (bytesRead <= 0)
			return (eraseSocket(sock, true));

		buffer[bytesRead] = '\0';
		sock->getRequest()->parseRequest(buffer);
		
		if (sock->getRequest()->getStatus() == FINISH_PARSED)
			return (modifyEvent(sock, 1));

		sock->setResponse(sock->getResponse()->makeResponse(sock->getRequest()));
		sock->setLastActivity(time(NULL));
}

/*SEND 
	send a message to the connection
	int send(int sockfd, const void *msg, int len, int flags); 
	MSG_DONTWAIT - Description: Enables non-blocking mode for this operation only. If the send operation would block, it fails with EAGAIN or EWOULDBLOCK.
	MSG_MORE - The caller has more data to send. This flag is used to indicate that more data is coming, and the protocol should not send the message immediately.
*/
void	Cluster::sendConnection(Socket *sock)
{
	size_t	bytes = send(sock->getSockFd(), sock->getResponseLine().c_str(), BUFFER_SIZE, 0); // a flag??
	if (bytes <= 0)
		return (eraseSocket(sock, true));

	sock->getResponseLine().erase(0, bytes);
	sock->setLastActivity(time(NULL));

	if (sock->getResponseLine().empty() && !sock->getRequest()->getConnectionKeepAlive()) 
		return (eraseSocket(sock, false));
	else if (sock->getResponseLine().empty())
	{
		cleanSocket(sock);
		return (modifyEvent(sock, 0));
	}
}

// if flag 0 - to in, 1 - to out
void	Cluster::modifyEvent(Socket *sock, bool flag)
{
	if (flag == 1)
		_ev.events = EPOLLOUT;
	else
		_ev.events = EPOLLIN;
	_ev.data.fd = sock->getSockFd();
	_ev.data.ptr = sock;
	epoll_ctl(_epFd, EPOLL_CTL_MOD, sock->getSockFd(), &_ev);
}

// 0 - not an error, 1 - closing because of the error
void	Cluster::eraseSocket(Socket *sock, bool err)
{
	std::string errmsg;
	
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, sock->getSockFd(), NULL) == -1)
		throw std::runtime_error("Error: epoll delete failed: " + errmsg.assign(strerror(errno)));
    close(sock->getSockFd());
	
	if (err)
		std::cerr << "Couldn't read from a socket: " + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
	else
		std::cout << "Client socket disconnected: "  + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
	
		// Find the iterator to the element
	std::vector<Socket>::iterator it = _sockets.begin();
	for (; it != _sockets.end(); it++) 
	{
		if (it->getSockFd() == sock->getSockFd()) 
		{
			_sockets.erase(it);
			std::cout << "Client socket eliminated from: " + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
			return ;
		}
	}
    // If the element is found, erase it
	std::cerr << "Couldn't eliminate a socket, socket not found: " + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
}

std::vector<Socket>::iterator	Cluster::eraseSocket(std::vector<Socket>::iterator sock)
{
	std::string errmsg;
	
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, sock->getSockFd(), NULL) == -1)
		throw std::runtime_error("Error: epoll delete failed: " + errmsg.assign(strerror(errno)));
    close(sock->getSockFd());
	std::cout << "Client socket disconnected for timeout: "  + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
    return (_sockets.erase(sock));
}

void	Cluster::cleanSocket(Socket *sock)
{
	sock->getResponseLine().clear();
	sock->getRequest()->cleanRequest();
	sock->getResponse()->cleanResponse();
}

void	Cluster::checkTimeout()
{
	time_t now = time(NULL);
	for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end();) 
	{
		if (now - it->getLastActivity() > TIMEOUT && !it->getMaster())
			it = eraseSocket(it);
		else
			++it;
	}
}