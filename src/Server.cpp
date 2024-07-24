#include "Server.hpp"

Server::Server(ServerConfig &sconfig) 
{
	initParamsServer(sconfig);
}

Server::Server(const Server& src) 
{
	*this = src;
}

Server& Server::operator=(const Server& src) 
{
	_ipAddress = src.getIpAdress();
	_ports = src.getPort();
	_maxBodySize = src.getMaxBodySize();
	_allowedMethods = src.getAllowedMethods();
	_errorPages = src.getErrorPages();
	_autoIndex = src.getAutoIndex();
	_return = src.getReturn();
	_cgiConf = src.getCgiConf();
	_serverName = src.getServerName();
	_locations = src.getLocationConfig();
	return (*this);
}

Server::~Server() 
{
}

void	Server::initParamsServer(ServerConfig &sconfig) 
{
	_ipAddress = sconfig.getIp();
	_ports = sconfig.getPort();
	_allowedMethods = sconfig.getAllowedMethods();
	_maxBodySize = sconfig.getMaxBodySize();
	_errorPages = sconfig.getErrorPages();
	_autoIndex = sconfig.getAutoIndex();
	_return = "";
	_cgiConf = sconfig.getCgiConf();
	_serverName = sconfig.getServerName();
}

// _____________  GETTERS _____________ 

const std::string& Server::getIpAdress() const 
{
	return (_ipAddress);
}

const std::vector<int>& Server::getPort() const 
{
	return (_ports);
}

const std::vector<std::string>& Server::getAllowedMethods() const{

	return (_allowedMethods);
}

size_t Server::getMaxBodySize() const {
	return (_maxBodySize);
}

const std::map<int, std::string>& Server::getErrorPages() const 
{
	return (_errorPages);
}

bool Server::getAutoIndex() const 
{
	return (_autoIndex);
}

const std::string& 	Server::getReturn() const 
{
	return (_return);
}

const std::map<std::string, std::string>&  Server::getCgiConf() const 
{
	return (_cgiConf);
}

const std::vector<std::string>& Server::getServerName() const
{
	return (_serverName);
}

const std::vector<LocationConfig> Server::getLocationConfig() const
{
	return (_locations);
}

// _____________  SETTERS _____________ 

void Server::setErrorPages(const std::map<int, std::string>&  errorPages) 
{
	_errorPages = errorPages;
}

void Server::setAutoIndex(bool autoindex) 
{
	_autoIndex = autoindex;
}

void Server::setReturn(const std::string& alias) 
{
	_return = alias;
}

void Server::setCgiConf(const std::map<std::string, std::string>& cgiConf)
{
	_cgiConf = cgiConf;
}
