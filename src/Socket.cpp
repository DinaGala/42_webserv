#include "Socket.hpp"

Socket::Socket(Server &server, int port) : _server(server), _port(port)
{
	_ipAddress = server.getIpAdress();
}

Socket::~Socket() {
}

void	Socket::setUpSocket(){
	initSocket();
//	bindSocket();
//	listenConnection();
}

/*SOCKET
	int socket(int domain, int type, int protocol);
	DOMAIN: communication domain - protocolos family  that socket will belong to. 
	AF_INET - IPv4 Internet protocols */
void	Socket::initSocket() 
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
    if (listen(_fd, SOMAXCONN) == -1) // we set our maximum?
	{
        close(_fd);
        throw std::runtime_error("Error: listen failed for " + _ipAddress + ":" + ft_itoa(_port));
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

void	Socket::bindSocket(){
	_sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(this->_ipAddress.c_str()); //tansform address to binary
	_sockaddr.sin_port = htons(this->_port); // htons is necessary to convert a number to network byte order

	if (bind(this->_sockfd, (struct sockaddr*)&this->_sockaddr, sizeof(this->_sockaddr)) < 0) {
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*LISTEN 
	int listen(int sockfd, int backlog);
	mark the socket as a passive socket, the Socket indicates that it is ready to accept connections
	backlog - maximum number of connections that will be queued before connections start being refused.
*/
void	Socket::listenConnection(){
	if (listen(this->_sockfd, 10000) < 0) {  //max num of connections??
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

//SETTERS
void	Socket::setIpAddress(const std::string & ipAddress){
	this->_ipAddress = ipAddress;
}

void	Socket::setPort(const int port){
	this->_port = port;
}

//GETTERS
long	Socket::getSockfd() const {
	return(_sockfd);
}

const struct sockaddr_in& Socket::getSockaddr() const {
	return(_sockaddr);
}

const Server& Socket::getServer() const {
	return(_server);
}

const std::string& Socket::getIpAdress() const {
	return (_ipAddress);
}

int Socket::getPort() const {
	return (_port);
}
