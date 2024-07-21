#include "Socket.hpp"

Socket::Socket(Server &server, int port): _server(server), _req(server), _port(port), _master(true)
{
	_ipAddress = server.getIpAdress();
	_lastActivity = time(nullptr);
	initMaster();
	setNonBlocking();
}

Socket::Socket(Server &server, Socket *sock): _server(server), _req(server), _master(false)
{
	_ipAddress = server.getIpAdress();
	_port = sock->getPort();
	initClient(sock->getSockFd());
	setNonBlocking();
}

Socket::~Socket() {
}

Socket& Socket::operator=(const Socket& src)
{
	_server = src._server;
	_port = src._port;
	_ipAddress = src._ipAddress;
	_fd = src._fd;
	_master = src._master;
//	_nClients = src._nClients;
	_req = src._req;
	_resp = src._resp;
	return (*this);
}

Socket::Socket(const Socket& src): _server(src._server), _req(src._server)
{
	*this = src;
}


void	Socket::setNonBlocking()
{
	// Get current socket flags
	int flags = fcntl(_fd, F_GETFL, 0);  
	if (flags == -1) 
		throw std::runtime_error("Error: fcntl get flags failed for " + _ipAddress + ":" + ft_itoa(_port));
	
	// Set non-blocking flag
	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("Error: fcntl set non blocking failed for " + _ipAddress + ":" + ft_itoa(_port));
}

/*SOCKET
	int socket(int domain, int type, int protocol);
	DOMAIN: communication domain - protocolos family  that socket will belong to. 
	AF_INET - IPv4 Internet protocols */

void	Socket::initMaster() 
{
	struct addrinfo hints, *servinfo, *p;
	std::string errmsg;
 //   int _fd;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; //SURE???

    if (getaddrinfo(_ipAddress.c_str(), ft_itoa(_port).c_str(), &hints, &servinfo) != 0)
        throw std::runtime_error("Error: getaddrinfo failed: " + errmsg.assign(gai_strerror(errno)));
    for (p = servinfo; p != NULL; p = p->ai_next) 
	{
        _fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_fd == -1) 
			continue;
        if (bind(_fd, p->ai_addr, p->ai_addrlen) == 0) 
			break;
        close(_fd);
    }
    if (p == NULL)
        throw std::runtime_error("Error: getaddrinfo failed the conversion for " + _ipAddress + ":" + ft_itoa(_port));
    freeaddrinfo(servinfo);
    
	if (listen(_fd, MAX_CON) == -1) // we set our maximum?
	{
        close(_fd);
        throw std::runtime_error("Error: listen failed for " + _ipAddress + ":" + ft_itoa(_port));
    }

	// NOT SURE ABOUT IT
	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
	{
		close(_fd);
        throw std::runtime_error("Error: setsockopt failed for " + _ipAddress + ":" + ft_itoa(_port));
	}
}

	/*ACCEPT
	**int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
	**addrlen is now a value-result argument. 
	**It expects a pointer to an int that will be the size of addr. 
	**After the function is executed, the int refered by addrlen will be set to the size of the peer address.
	*/
	// Grab a connection from the queue
void	Socket::initClient(int masterfd) 
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	_fd = accept(masterfd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
	if (_fd < 0) {
		std::cerr << "Failed to grab connection. errno: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*SOCKET ADDRESS STRUCTURE 
	Struct used to specify the address we want to assign to the socket.
	The exact struct that needs to be used to define the address, varies by protocol. 
	Since we are going to use IP for this Socket, we will use sockaddr_in
	
	struct sockaddr_in {
	sa_family_t    sin_family; // address family: AF_INET
	in_port_t      sin_port;   // port in network byte order
	struct in_addr sin_addr;   // internet address
	};

  BIND
	int bind(int sockfd, const sockaddr *addr, socklen_t addrlen);
	bind - assign an IP address and port to the socket.
*/

// void	Socket::bindSocket(){
// 	_sockaddr.sin_family = AF_INET;
//     _sockaddr.sin_addr.s_addr = inet_addr(this->_ipAddress.c_str()); //tansform address to binary
// 	_sockaddr.sin_port = htons(this->_port); // htons is necessary to convert a number to network byte order

// 	if (bind(this->_sockfd, (struct sockaddr*)&this->_sockaddr, sizeof(this->_sockaddr)) < 0) {
// 		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }

/*LISTEN 
	int listen(int sockfd, int backlog);
	mark the socket as a passive socket, the Socket indicates that it is ready to accept connections
	backlog - maximum number of connections that will be queued before connections start being refused.
*/
// void	Socket::listenConnection(){
// 	if (listen(this->_sockfd, 10000) < 0) {  //max num of connections??
// 		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }

//SETTERS
void	Socket::setIpAddress(const std::string & ipAddress){
	this->_ipAddress = ipAddress;
}

void	Socket::setPort(const int port){
	this->_port = port;
}

void	Socket::setResponse(std::string response){
	this->_response = response;
}

void	Socket::setLastActivity(time_t now)
{
	_lastActivity = now;
}


//GETTERS
const int	Socket::getSockFd() const {
	return(_fd);
}

bool	Socket::getMaster() const {
	return(_master);
}

// const struct sockaddr_in& Socket::getSockaddr() const {
// 	return(_sockaddr);
// }

// const Server& Socket::getServer() const {
// 	return(_server);
// }

Server& Socket::getServer() const {
	return(_server);
}

const std::string& Socket::getIpAdress() const {
	return (_ipAddress);
}

const int Socket::getPort() const {
	return (_port);
}

Request* Socket::getRequest() {
	return (&_req);
}

Response* Socket::getResponse() {
	return (&_resp);
}

std::string& Socket::getResponseLine() {
	return (_ipAddress);
}

const time_t Socket::getLastActivity() const {
	return (_lastActivity);
}