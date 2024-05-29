/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:48:36 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/05/29 22:44:24 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}



ServerConfig::ServerConfig(std::ifstream &file)
{
	
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	_host = src._host;
	_hostName = src._hostName;
	_port = src._port; 
	_serverName = src._serverName;
	_rootDir = src._rootDir;
	_locations = src._locations;
	error_pages = src.error_pages;
	_max_body_size = src._max_body_size;
	cgiConf = src.cgiConf;
	autoindex = src.autoindex;
	empty = src.empty;
	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig& src)
{
	*this = src;
}

ServerConfig::~ServerConfig()
{
	_locations.clear();
	error_pages.clear();
	cgiConf.clear();
}

std::string ServerConfig::getHost() const
{
	return (_host);
}

std::string ServerConfig::getHostName() const
{
	return (_hostName);
}

int ServerConfig::getPort() const
{
	return (_port);
}

std::string ServerConfig::getServerName() const
{
	return (_serverName);
}

std::string ServerConfig::getRootDir() const
{
	return (_rootDir);
}

std::vector<LocationConfig> ServerConfig::getLocationConf() const
{
	return (_locations);
}

std::map<int, std::string> ServerConfig::getErrorPages() const
{
	return (error_pages);
}

size_t ServerConfig::getMaxBodySize() const
{
	return (_max_body_size);
}

std::vector<CgiConfig> ServerConfig::getCgiConf() const
{
	return (cgiConf);
}

bool ServerConfig::getAutoIndex() const
{
	return (autoindex);
}

// void ServerConfig::_initConfig();
// void ServerConfig::setHost(const std::string& host);
// void ServerConfig::setPort(int port);
// void ServerConfig::setServerName(const std::string& serverName);
// void ServerConfig::setRootDir(const std::string& rootDir);
// void ServerConfig::addLocationConfig(const LocationConfig& locations);
// void ServerConfig::addErrorPage(int code, const std::string& page);
// void ServerConfig::setErrorPages(const std::map<int, std::string>& error_pages);
// void ServerConfig::setClientMaxBodySize(size_t client_max_body_size);
// void ServerConfig::setCgiConf(const std::vector<CgiConfig>& cgiConf);
// void ServerConfig::addCgiConfig(const CgiConfig& cgiConf);
// void ServerConfig::setAutoIndex(bool autoindex);
// void ServerConfig::setHostName(const std::string& hostName);
