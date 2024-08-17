
#include "Cluster.hpp"

Cluster::Cluster() 
{
}

Cluster::~Cluster() 
{
	
	std::string errmsg;
	for (std::list<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); it++) 
	{
		if (epoll_ctl(_epFd, EPOLL_CTL_DEL, it->getSockFd(), NULL) == -1)
			throw std::runtime_error("Error: epoll delete failed: " + errmsg.assign(strerror(errno)));
		close(it->getSockFd());
	}
	close(_epFd);
	_sconf.clear();
	_servers.clear();
	_sockets.clear();
		// int fd[2];
		// pipe(fd);
		// std::cout << "fds are: " << fd[0] << "   1: " << fd[1] << std::endl;
		// close(fd[0]);
		// close(fd[1]); 
	std::cout << "\n\033[33;1mBYE BYE BABY!\033[0m\n";
}

void	Cluster::setUpCluster(int ac, char **av){
	std::string filename = "conf/default.conf";

	if (ac == 2)
		filename = av[1];
	
	_sconf = Parse::configParse(filename.c_str());
	std::cout << _sconf;
	createServers();
	createSockets();
	createEpoll();
}

void	Cluster::createServers()
{
	for (std::vector<ServerConfig>::iterator it = _sconf.begin(); it != _sconf.end(); it++) {
		Server server(*it);
		this->_servers.push_back(server);
	}
}

void	Cluster::createSockets()
{
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
	for (std::list<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); it++) 
	{
		_ev.data.fd = it->getSockFd();
		_ev.data.ptr = &(*it);
		if (epoll_ctl(_epFd, EPOLL_CTL_ADD, it->getSockFd(), &_ev) == -1) 
			throw std::runtime_error("Error: master epoll control failed: " + errmsg.assign(strerror(errno)));
	}
}

/* This function checks, for all the pids stored:
(1) if the process has finished by itself
	L if so, it sets the response's code + the cgi fd (from which we get the cgi's response)
(2) if the process running time is greater than the TIMEOUT
	L if so, it kills the process
If any of this happens, the event for that socket will be modified to EPOLLOUT
If an error happens, the response's code will be set accordingly and the event
will be modified to EPOLLOUT.
*/
void	Cluster::_checkChilds(void)
{
	int 	status;
	int 	wait;
	Socket	*sock;
	std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >::iterator it = this->_pids.begin();

	while (it != this->_pids.end())
	{
		sock = it->second.second;
		wait = waitpid(it->first, &status, WNOHANG);
		if (wait == -1)
		{
			std::cout << "\033[1;31mwaitpid failed\n\033[0m";
			sock->getResponse()->setCode(500);
			modifyEvent(sock, 1);
			this->_pids.erase(it);
		}
		else if (WIFEXITED(status) && !WEXITSTATUS(status))
		{
			std::cout << "\033[1;31mit has WEXITED: status: " << WEXITSTATUS(status) * 10 << "\n\033[0m";
			sock->getResponse()->setCode(WEXITSTATUS(status) * 10);
			sock->getResponse()->setCgiFd(it->second.first.second);
			modifyEvent(sock, 1);
			this->_pids.erase(it);
		}
		else if (std::time(NULL) - it->second.first.first > TIMEOUT) //if timeout is reached kill the child
		{
			std::cout << "\033[1;31mTIMEOUT pid: " << it->first << "\n\033[0m";
			if (kill(it->first, SIGKILL))// kill fail
				sock->getResponse()->setCode(500);
			else
			{
				waitpid(it->first, &status, 0); //wait until the child is actually dead
				sock->getResponse()->setCode(504);
			}
			modifyEvent(sock, 1);
			this->_pids.erase(it);
		}
		it++;
	}
}

/* This function looks for a specific Socket* in this->_pids
and returns the element (iterator) containing it.
If Socket* is not found, it resturns this->_pids.end().
*/
std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >::iterator
Cluster::findPidFromSocket(Socket *sock)
{
	std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >::iterator it = this->_pids.begin();

	while (it != this->_pids.end())
	{
		if (it->second.second == sock)
			return (it);
		it++;
	}
	return (it);
}

/* ADDITION: If the client hangs up (EPOLLHUP) and there's
a pid linked to that Socket* , we kill it.
The problem is, it only works (at least for me) when we're printing stuff (EPOLLHUP, EPOLLIN, EPOLLOUT)
*/
void	Cluster::runCluster()
{
	std::string errmsg;
	std::map<pid_t, std::pair<std::pair<std::time_t, int>, Socket *> >::iterator it;
	int	status;
	initSignals();
	
	while (signaled)
	{ 
		_nfds = epoll_wait(_epFd, _events, MAX_EVENTS, 200); 

		if (_nfds == -1) {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("Error: epoll wait failed: " + errmsg.assign(strerror(errno)));
		}
		std::cout << "\033[32;1mWAITING\033[0m\n";
		std::cout << "\033[32;1mnfds: " << _nfds << "\033[0m\n";
		for (int n = 0; n < _nfds; ++n)
		{
			//std::cout << "All sockets:\n" << _sockets;
			Socket *cur = static_cast<Socket *>(_events[n].data.ptr);
			//std::cout << "current socket is:  " << cur << "\n" << *cur;

			/****** ADDED BY NURIA ********/
			if (_events[n].events & EPOLLHUP)
			{
				write(2, "epollhup\n", 9);
				it = findPidFromSocket(cur);
				if (it != this->_pids.end())
				{
					write(2, "found\n", 6);
					if (kill(it->first, SIGKILL))
						cur->getResponse()->setCode(500);
					else
					{
						waitpid(it->first, &status, 0); //wait until the child is actually dead
						cur->getResponse()->setCode(504);
					}
					modifyEvent(cur, 1);
					this->_pids.erase(it);
				}
			}
			/*****************************/
			else if (_events[n].events & EPOLLIN)
			{
				write(2, "epollin\n", 8);
				//std::cerr << "\033[1;31mCluster: EPOLLIN\033[0m" << std::endl;
				if (cur->getMaster())
				{
					write(2, "create\n", 7);
					//EPOLLIN
					acceptConnection(cur);
				}
				else
				{
					write(2, "else\n", 5);
					//EPOLLIN, if all read - change to EPOLLOUT 
					readConnection(cur);
				}
			}
			else if (_events[n].events & EPOLLOUT)
			{
				write(2, "epollout\n", 9);
			// send in chunks and change to epollin when finished. New request? OR close? 
				sendConnection(cur);
			}
			else //ADD timeout
				throw std::runtime_error("Error: epoll event error ");
		}
		if (this->_pids.size() != 0)
			this->_checkChilds();
		//checkTimeout();
//		usleep(1000);
	}
//	std::cout << "----- END OF LOOP -----" << std::endl;
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
	std::list<Socket>::iterator it = _sockets.end();
    --it;

	_ev.events = EPOLLIN;
	_ev.data.fd = socket.getSockFd();
	_ev.data.ptr = &(*it);
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, socket.getSockFd(), &_ev) == -1) 
		throw std::runtime_error("Error: client epoll control failed: " + errmsg.assign(strerror(errno)));

}

void	Cluster::readConnection(Socket *sock)
{
		if (sock->getRequest()->getStatus() == FINISH_PARSED)
			return ;
		std::vector<unsigned char> buffer(BUFFER_SIZE);
		int bytesRead = recv(sock->getSockFd(), buffer.data(), BUFFER_SIZE, MSG_DONTWAIT);
		std::cout << "bytes read: " << bytesRead << std::endl;
		if (bytesRead <= 0 && sock->getRequest()->getStatus() != FINISH_PARSED)
		{
			std::cout <<  "msg from read\n";
			return (eraseSocket(sock, true));
		}
		sock->getRequest()->parseRequest(buffer, bytesRead);
		std::cout << "\033[1;36mREQUEST: \n" << *(sock->getRequest()) << "\033[0m";
		if (sock->getRequest()->getStatus() == FINISH_PARSED)
		{
			/****** ADDED BY NURIA ********/
			if (sock->getRequest()->getCgi() == true)
			{
				Cgi	cgi(*(sock->getRequest()));
				int err = cgi.executeCgi();
				if (err != 0)
				{
					sock->getResponse()->setCode(err);
					modifyEvent(sock, 1);
				}
				else
					this->_pids[cgi.getPid()] = std::make_pair(std::make_pair(std::time(NULL), cgi.getCgiFd()), sock);
			}
			else
				modifyEvent(sock, 1);
			/*****************************/
		}
		else
			return ;
		//sock->setResponse(sock->getResponse()->makeResponse(sock->getRequest()));
		//std::cout << "\033[35;1mRESPONSE: \n" << *(sock->getResponse()) << "\033[0m";
//		sock->setLastActivity(time(NULL));
}

/*SEND 
	send a message to the connection
	int send(int sockfd, const void *msg, int len, int flags); 
	MSG_DONTWAIT - Description: Enables non-blocking mode for this operation only. If the send operation would block, it fails with EAGAIN or EWOULDBLOCK.
	MSG_MORE - The caller has more data to send. This flag is used to indicate that more data is coming, and the protocol should not send the message immediately.
*/
void	Cluster::sendConnection(Socket *sock)
{
	size_t	bytes;

	std::cerr << "\033[34;1mSEND CONNECTION" << "\033[0m\n";
	if (sock->getResponse()->getDone() == false)
		sock->setResponse(sock->getResponse()->makeResponse(sock->getRequest()));
	std::cout << "\033[35;1mRESPONSE: \n" << *(sock->getResponse()) << "\033[0m";
	if (sock->getResponseLine().size() < BUFFER_SIZE)
		bytes = send(sock->getSockFd(), sock->getResponseLine().c_str(), sock->getResponseLine().size(), MSG_DONTWAIT); // a flag??
	else
		bytes = send(sock->getSockFd(), sock->getResponseLine().c_str(), BUFFER_SIZE, MSG_DONTWAIT); // a flag??	
	if (bytes <= 0)
		return (eraseSocket(sock, true));
	std::cout << "\033[34;1mBYTES " << bytes << "\033[0m\n";

	sock->getResponseLine().erase(0, bytes);
//	sock->setLastActivity(time(NULL));
	if (sock->getResponseLine().empty() && !sock->getRequest()->getConnectionKeepAlive()) 
		return (eraseSocket(sock, false));
	else if (sock->getResponseLine().empty())
	{
	//	std::cout << "\033[1;33mCleaning socket\033[0m" << std::endl;
	//	eraseSocket(sock, false);
	//	shutdown(sock->getSockFd(), SHUT_WR);


		cleanSocket(sock);
	//	exit (0);
		modifyEvent(sock, 0);
	//	exit (0);
	}
//	exit (0);
}

// if flag 0 - to in, 1 - to out
void	Cluster::modifyEvent(Socket *sock, bool flag)
{
	if (flag)
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
	
//	std::cout << "Eliminamos el socket: " << sock << "\n" << *sock;
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, sock->getSockFd(), NULL) == -1)
		throw std::runtime_error("Error: epoll delete failed: " + errmsg.assign(strerror(errno)));
    close(sock->getSockFd());
	
	if (err)
		std::cerr << "\033[31;1mCouldn't read from a socket: " + sock->getIpAdress() + ":" << sock->getPort() << "\033[0m" << std::endl;
	else
		std::cout << "\033[31;1mClient socket disconnected: "  + sock->getIpAdress() + ":" << sock->getPort() << "\033[0m" << std::endl;
	
		// Find the iterator to the element
	std::list<Socket>::iterator it = _sockets.begin();
	for (; it != _sockets.end(); it++) 
	{
		if (it->getSockFd() == sock->getSockFd()) 
		{
		//	std::cout << "Eliminamos el socket: " << &(*it) << "\n" << *it;
			std::cout << "\033[31;1mClient socket eliminated from: " + it->getIpAdress() + ":" << it->getPort() << "\033[0m" << std::endl;
			_sockets.erase(it);
			return ;
		}
	}
    // If the element is found, erase it
	std::cerr << "Couldn't eliminate a socket, socket not found: " + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
}

std::list<Socket>::iterator	Cluster::eraseSocket(std::list<Socket>::iterator sock)
{
	std::string errmsg;
	
//	std::cout 
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, sock->getSockFd(), NULL) == -1)
		throw std::runtime_error("Error: epoll delete failed: " + errmsg.assign(strerror(errno)));
    close(sock->getSockFd());
	std::cout << "Client socket disconnected for timeout: "  + sock->getIpAdress() + ":" << sock->getPort() << std::endl;
    return (_sockets.erase(sock));
}

void	Cluster::cleanSocket(Socket *sock)
{
	sock->getResponseLine().clear();
	sock->cleanRequestResponse();
//	sock->getRequest()->initParams(); //modified by Julia
//	sock->getResponse()->cleanResponse();
}

// void	Cluster::checkTimeout()
// {
// 	time_t now = time(NULL);
// 	for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end();) 
// 	{
// 		if (now - it->getLastActivity() > TIMEOUT && !it->getMaster())
// 			it = eraseSocket(it);
// 		else
// 			++it;
// 	}
// }

std::ostream	&operator<<(std::ostream &out, const Socket &val)
{
//	out << "Port:  " << val.getServer() << "\n";
    out << "Port:  " << val.getPort() << "\n";
    out << "IP:  " << val.getIpAdress() << "\n";
    out << "Socket fd:  " << val.getSockFd() << "\n";
//    out << "Last activity:  " << val.getLastActivity() << "\n";
    out << "Master:  " << val.getMaster() << "\n\n";
 //   out << "Error pages:  \n" << val.getResponseLine() << "\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::list<Socket> &val)
{
 	std::list<Socket> temp = val;
    for (std::list<Socket>::iterator it = temp.begin(); it != temp.end(); it++) {
        out << "SOCKET ----------------------" << "\n" << (*it) << "\n";
    }
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const Request &val)
{
    out << "Status:  " << val.getStatus() << "\n";
    out << "Code:  " << val.getCode() << "\n";
    out << "Path:  " << val.getPath() << "\n";
	out << "CGI:  " << val.getCgi() << "\n";
	out << "Autoindex:  " << val.getAutoIndex() << "\n";
	out << "Request line:  " << val.getRequestLine() << "\n";
	out << "Allow methods:  " << val.getAllowedMethods() << "\n";
	out << "Number Location:  " << val.getPosLocation() << "\n";
	out << "Status:  " << val.getStatus() << "\n";
	out << "\n\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const Response &val)
{
    //	out << "Port:  " << val.getServer() << "\n";
	  //out << "Response:  " << val.getResponse() << "\n";
    out << "Code:  " << val.getCode() << "\n";
    // out << "Socket fd:  " << val.getSockFd() << "\n";
    // out << "Last activity:  " << val.getLastActivity() << "\n";
    // out << "Master:  " << val.getMaster() << "\n\n";
 //   out << "Error pages:  \n" << val.getResponseLine() << "\n";

	return (out);
}
