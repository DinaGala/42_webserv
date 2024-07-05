/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:48:36 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/05 14:38:32 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

// _____________  CONSTRUCTORS ______________________________________

ServerConfig::ServerConfig(): loc(true) {}

ServerConfig::ServerConfig(std::string file): loc(true)
{
//	std::cout << "NEWSERV:" << "\n" << file << " ----------------------------------------------" << std::endl;
	_initKeys();
	_autoIndex = false;
	_root = "html";
	_maxBodySize = 10000000;
	_cgiConf[".sh"] = "/bin/bash";
	_cgiConf[".js"] = "/usr/bin/node";
	_cgiConf[".php"] = "/usr/bin/php";
	_cgiConf[".py"] = "/usr/bin/python3";
	Parse::complexParse<ServerConfig>(*this, file);
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	_host = src._host;
//	_hostName = src._hostName;
	_port = src._port;
	_serverName = src._serverName;
	_root = src._root;
	_ip = src._ip;
	_allowedMethods = src._allowedMethods;
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
   	_keys["client_max_body_size"] = &Parse::bodySizeParse;
   	_keys["autoindex"] = &Parse::autoIndexParse<ServerConfig>;
    _keys["error_page"] = &Parse::errorParse<ServerConfig>;
    _keys["allow_methods"] = &Parse::allowMethodsParse<ServerConfig>;
 	_keys["cgi"] = &Parse::cgiParse<ServerConfig>;

	_vars["port"] = false;
	_vars["host"] = false;
	_vars["ip"] = false;
	_vars["server_name"] = false;
    _vars["root"] = false;
   	_vars["client_max_body_size"] = false;
   	_vars["autoindex"] = false;
    _vars["error_page"] = false;
    _vars["allow_methods"] = false;
 	_vars["cgi"] = false;
	_vars["location"] = false;
}

// _____________  GETTERS ______________________________________

const std::string ServerConfig::getHost() const
{
	return (_host);
}

const std::string ServerConfig::getIp() const
{
	return (_ip);
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

const std::map<std::string, bool>& 	ServerConfig::getVars()
{
    return (_vars);
}

// _____________  SETTERS ______________________________________

void ServerConfig::setHost(const std::string& host)
{
	if (_vars["host"] || _vars["ip"])
		throw std::invalid_argument("Error: host duplication in listen: " + host);
	_host = host;
	_vars["host"] = true;
	if (Parse::ipCheck(host))
		setIp(host);
}

void ServerConfig::setIp(const std::string& ip)
{
	_ip = ip;
	_vars["ip"] = true;
}

void ServerConfig::setPort(int port)
{
	for (std::vector<int>::iterator it = _port.begin(); it != _port.end(); it++)
	{
		if (*it == port)
			throw std::invalid_argument("Error: port duplication in listen");
	}
	_port.push_back(port);
	if (!_vars["port"])
		_vars["port"] = true;
}

void ServerConfig::setServerName(const std::string& serverName)
{
	_serverName.push_back(serverName);
	_vars["server_name"] = true;
}

void ServerConfig::setRoot(const std::string& root)
{
	if (_vars["root"])
		throw std::invalid_argument("Error: \"root\" directive is duplicate");
	_root = root;
	_vars["root"] = true;	
}

void ServerConfig::setLocationConfig(const LocationConfig& location)
{
//	std::cout << "ENTERED SET LOCATION ----------- " << std::endl;
	for (std::vector<LocationConfig>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if (it->getUri() == location.getUri())
			throw std::invalid_argument("Error: uri duplication in location: " + location.getUri());
	}
	_locations.push_back(location);
	if (!_vars["location"])
		_vars["location"] = true;
}

void ServerConfig::setErrorPage(int code, const std::string& page)
{
	_errorPages[code] = page;
	if (!_vars["error_page"])
		_vars["error_page"] = true;
//	_errorPages.insert(std::make_pair(code, page));
}

void ServerConfig::setClientMaxBodySize(size_t client_max_body_size)
{
	if (_vars["client_max_body_size"])
		throw std::invalid_argument("Error: \"client_max_body_size\" directive is duplicate ");
	_maxBodySize = client_max_body_size;
	_vars["client_max_body_size"] = true;
}

void ServerConfig::setCgiConf(const std::string &ext, const std::string &path)
{
	_cgiConf[ext] = path;
	if (!_vars["cgi"])
		_vars["cgi"] = true;
}

void ServerConfig::setAutoIndex(bool autoindex)
{
	if (_vars["autoindex"])
		throw std::invalid_argument("Error:\"autoindex\" directive is duplicate ");
	_autoIndex = autoindex;
	_vars["autoindex"] = true;
}

void 	ServerConfig::setAllowMethod(const std::string& method)
{
//	std::cout << "SET ALLOW METHODS, METHOD: " << method << std::endl;
	for (std::vector<std::string>::iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); it++)
	{
		if (*it == method)
			throw std::invalid_argument("Error: method duplication in allowed_method");
	}
	_allowedMethods.push_back(method);
//	std::cout << "END SET ALLOW METHODS, VARIABLE: " << _allowedMethods << std::endl;
}

void 		ServerConfig::setVars(const std::string& key)
{
	_vars[key] = true;
}

// void ServerConfig::setHostName(const std::string& hostName)
// {
// 	_hostName = hostName;
// }

// void ServerConfig::setErrorPages(const std::map<int, std::string>& error_pages)
// {

// }

// void ServerConfig::setCgiConf(const std::vector<CgiConfig>& cgiConf)
// {

// }

// void ServerConfig::_initConfig();

