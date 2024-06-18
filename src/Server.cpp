#include "Server.hpp"
#include <fcntl.h>

Server::Server() {
	//SERVER CONFIG
	setIpAddress("127.0.0.1");
	setPort(8080);
}

Server::~Server() {
}

void	Server::setUpServer(){
	initSocket();
	bindSocket();
	listenConnection();
}

/*SOCKET
int socket(int domain, int type, int protocol);
DOMAIN: communication domain - protocolos family  that socket will belong to. 
AF_INET - IPv4 Internet protocols */
void	Server::initSocket() {
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*SOCKET ADDRESS STRUCTURE 
	Struct used to specify the address we want to assign to the socket.
	The exact struct that needs to be used to define the address, varies by protocol. 
	Since we are going to use IP for this server, we will use sockaddr_in
	
	struct sockaddr_in {
	sa_family_t    sin_family; // address family: AF_INET
	in_port_t      sin_port;   // port in network byte order
	struct in_addr sin_addr;   // internet address
	};
*/
/*BIND
int bind(int sockfd, const sockaddr *addr, socklen_t addrlen);
bind - assign an IP address and port to the socket.
*/

void	Server::bindSocket(){
	_sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(this->_ipAddress); //tansform address to binary
	_sockaddr.sin_port = htons(this->_port); // htons is necessary to convert a number to network byte order

	if (bind(this->_sockfd, (struct sockaddr*)&this->_sockaddr, sizeof(this->_sockaddr)) < 0) {
		std::cout << "Failed to bind to port 8080. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*LISTEN 
	int listen(int sockfd, int backlog);
	mark the socket as a passive socket, the server indicates that it is ready to accept connections
	backlog - maximum number of connections that will be queued before connections start being refused.
*/
void	Server::listenConnection(){
	if (listen(this->_sockfd, 10000) < 0) {  //max num of connections??
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

//SETTERS
void	Server::setIpAddress(const char* ipAddress){
	this->_ipAddress = ipAddress;
}

void	Server::setPort(const int port){
	this->_port = port;
}

//GETTERS
long	Server::getSockfd() const {
	return(_sockfd);
}

struct sockaddr_in Server::getSockaddr() const {
	return(_sockaddr);
}
/*
int	Server::executeCgi(void)
{
	pid_t	pid;
	int		status;
	int		pipefd[2];

	if (pipe(pipefd))
	{
		std::cerr << "Fuck pipe failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	char **args = (char **)calloc(2, sizeof(char *));
	args[0] = (char *)calloc(20, sizeof(char *));
	char tmp[] = "./cgi/test";
	for (int i = 0; tmp[i]; i++)
		args[0][i] = tmp[i];

	std::cerr << "before dup" << std::endl;
	if (dup2(pipefd[1], 1))
	{
		std::cout << "Fuck dup failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cerr << "after dup" << std::endl;
	write(1, "hola\n", 5);
	write(2, "holi\n", 5);
	//dup2(pipefd[1], 1);
	pid = fork();
	std::cerr << "before child" << std::endl;
	if (pid == 0)
	{
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
		execve(args[0], args, NULL);
		std::cerr << "Fuck something happened" << std::endl;
		exit(127);
	}
	std::cerr << "after child" << std::endl;
	close(pipefd[0]);
	close(pipefd[1]);
	if (waitpid(pid, &status, 0) == pid)
	{
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status);
		else if (WIFSTOPPED(status))
			status = WSTOPSIG(status);
	}
	std::cerr << "status: " << status << std::endl;
	return (status);
}*/

#include <stdio.h>

int Server::executeCgi(void)
{
    pid_t pid;
    int status;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        std::cerr << "Pipe failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    char **args = (char **)calloc(3, sizeof(char *));
    args[0] = (char *)calloc(20, sizeof(char));
    args[1] = (char *)calloc(20, sizeof(char));
    const char tmp[] = "/usr/bin/python3";
    const char tmp2[] = "./test.py";
    strcpy(args[0], tmp);
    strcpy(args[1], tmp2);

    pid = fork();
    if (pid == -1)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // Child process
    {
        close(pipefd[0]); // Close unused read end
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "dup2 failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // Close write end after dup

        execve(args[0], args, NULL);
        std::cerr << "Execve failed" << std::endl;
		perror("execve");
        exit(EXIT_FAILURE);
    }
    else // Parent process
    {
        close(pipefd[1]); // Close unused write end
        char buffer[1024];
        ssize_t count;

        while ((count = read(pipefd[0], buffer, sizeof(buffer))) != 0)
        {
            if (count == -1)
            {
                std::cerr << "Read failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout.write(buffer, count);
        }
        close(pipefd[0]); // Close read end

        if (waitpid(pid, &status, 0) == -1)
        {
            std::cerr << "Waitpid failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status))
            status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            status = WTERMSIG(status);
        else if (WIFSTOPPED(status))
            status = WSTOPSIG(status);

        std::cerr << "status: " << status << std::endl;
    }

    // Free allocated memory
    free(args[0]);
    free(args);

    return status;
}
