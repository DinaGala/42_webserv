/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:48:36 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/26 21:39:37 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

// _____________  CONSTRUCTORS ______________________________________

ServerConfig::ServerConfig(): loc(true) {}

ServerConfig::ServerConfig(std::string file): loc(true)
{
//	std::cout << "NEWSERV:" << "\n" << file << " ----------------------------------------------" << std::endl;
	_initKeys();
	Parse::complexParse<ServerConfig>(*this, file);
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

ServerConfig::ServerConfig(const ServerConfig& src): loc(true)
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

void ServerConfig::_initKeys()
{
	_keys["listen"] = &Parse::hostParse;
	_keys["server_name"] = &Parse::servNameParse;
    _keys["root"] = &Parse::rootParse<ServerConfig>;
   	_keys["max_body_size"] = &Parse::bodySizeParse;
   	_keys["autoindex"] = &Parse::autoIndexParse<ServerConfig>;
    _keys["error_page"] = &Parse::errorParse<ServerConfig>;
    _keys["allow_methods"] = &Parse::allowMethodsParse<ServerConfig>;
 	_keys["cgi"] = &Parse::cgiParse<ServerConfig>;
}

// _____________  GETTERS ______________________________________

const std::string ServerConfig::getHost() const
{
	return (_host);
}

const std::string ServerConfig::getHostName() const
{
	return (_hostName);
}

const std::vector<int> ServerConfig::getPort() const
{
	return (_port);
}

const std::vector<std::string> ServerConfig::getServerName() const
{
	return (_serverName);
}

const std::string ServerConfig::getRoot() const
{
	return (_root);
}

const std::vector<LocationConfig> ServerConfig::getLocationConfig() const
{
	return (_locations);
}

const std::map<int, std::string> ServerConfig::getErrorPages() const
{
	return (_errorPages);
}

size_t ServerConfig::getMaxBodySize() const
{
	return (_maxBodySize);
}

const std::map<std::string, std::string>  ServerConfig::getCgiConf() const
{
	return (_cgiConf);
}

bool ServerConfig::getAutoIndex() const
{
	return (_autoIndex);
}

const std::vector<std::string> 		ServerConfig::getAllowedMethods() const
{
	return (_allowedMethods);
}

const std::map<std::string, ServerConfig::func>& 	ServerConfig::getKeys()
{
    return (_keys);
}

// _____________  SETTERS ______________________________________

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

