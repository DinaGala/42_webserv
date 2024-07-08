#include "Server.hpp"

Server::Server(ServerConfig sconfig) {
	_ipAddress = sconfig.getIp();
	_ports = sconfig.getPort();
	_allowedMethods = sconfig.getAllowedMethods();
	_maxBodySize = sconfig.getMaxBodySize();
}

Server::~Server() {
}


const std::string Server::getIp() const
{
	return (_ipAddress);
}

const std::vector<int> Server::getPort() const
{
	return (_ports);
}

const std::vector<std::string> 		Server::getAllowedMethods() const
{
	return (_allowedMethods);
}


size_t Server::getMaxBodySize() const
{
	return (_maxBodySize);
}

