/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:48:36 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/25 21:48:54 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}


std::map<std::string, funcS> ServerConfig::_keys = {

	{"listen", &Parse::hostParse},
	{"server_name", &Parse::servNameParse},
    {"root", &Parse::rootParse},
   	{"max_body_size", &Parse::bodySizeParse},
   	{"autoindex", &Parse::autoIndexParse},
    {"error_pages", &Parse::errorParse},
    {"allow_methods", &Parse::allowMethodsParse},
 	{"cgi", &Parse::cgiParse}
	 //   {"location", 
};

ServerConfig::ServerConfig(std::string file)
{
	std::cout << "NEWSERV:" << "\n" << file << " ----------------------------------------------" << std::endl;
	Parse::complexParse(*this, file);
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	_host = src._host;
	_hostName = src._hostName;
	_port = src._port;
	_serverName = src._serverName;
	_root = src._root;
	_locations = src._locations;
	_errorPages = src._errorPages;
	_maxBodySize = src._maxBodySize;
	_cgiConf = src._cgiConf;
	_autoIndex = src._autoIndex;
//	empty = src.empty;
	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig& src)
{
	*this = src;
}

ServerConfig::~ServerConfig()
{
	_port.clear();
	_serverName.clear();
	_locations.clear();
	_allowedMethods.clear();
	_errorPages.clear();
	_cgiConf.clear();
}

std::string ServerConfig::getHost() const
{
	return (_host);
}

std::string ServerConfig::getHostName() const
{
	return (_hostName);
}

std::vector<int> ServerConfig::getPort() const
{
	return (_port);
}

std::vector<std::string> ServerConfig::getServerName() const
{
	return (_serverName);
}

std::string ServerConfig::getRoot() const
{
	return (_root);
}

std::vector<LocationConfig> ServerConfig::getLocationConfig() const
{
	return (_locations);
}

std::map<int, std::string> ServerConfig::getErrorPages() const
{
	return (_errorPages);
}

size_t ServerConfig::getMaxBodySize() const
{
	return (_maxBodySize);
}

std::map<std::string, std::string>  ServerConfig::getCgiConf() const
{
	return (_cgiConf);
}

bool ServerConfig::getAutoIndex() const
{
	return (_autoIndex);
}

void ServerConfig::setHost(const std::string& host)
{
	_host = host;
}
void ServerConfig::setPort(int port)
{
	_port.push_back(port);
}

void ServerConfig::setServerName(const std::string& serverName)
{
	_serverName.push_back(serverName);
}

void ServerConfig::setRoot(const std::string& root)
{
	_root = root;
}

void ServerConfig::setLocationConfig(const LocationConfig& location)
{
	_locations.push_back(location);
}

void ServerConfig::setErrorPage(int code, const std::string& page)
{
	_errorPages.insert(std::make_pair(code, page));
}

void ServerConfig::setClientMaxBodySize(size_t client_max_body_size)
{
	_maxBodySize = client_max_body_size;
}

void ServerConfig::setCgiConf(const std::string &ext, const std::string &path)
{
	_cgiConf.insert(std::make_pair(ext, path));
}

void ServerConfig::setAutoIndex(bool autoindex)
{
	_autoIndex = autoindex;
}

void ServerConfig::setHostName(const std::string& hostName)
{
	_hostName = hostName;
}

void 	ServerConfig::setAllowedMethod(const std::string& method)
{
	_allowedMethods.push_back(method);
}

// void ServerConfig::setErrorPages(const std::map<int, std::string>& error_pages)
// {

// }

// void ServerConfig::setCgiConf(const std::vector<CgiConfig>& cgiConf)
// {

// }

// void ServerConfig::_initConfig();

