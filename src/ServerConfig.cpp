/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:48:36 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/08 15:57:40 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerConfig.hpp"


std::map<int, std::pair<std::string, std::string> > ServerConfig::_initStatus()
{
	std::map<int, std::pair<std::string, std::string> > error;

	error[100] = std::make_pair("Continue", "");
	error[200] = std::make_pair("OK", "");
	error[201] = std::make_pair("Created", "");
	error[204] = std::make_pair("No Content", "");
	error[301] = std::make_pair("Moved Permanently", "");
	error[302] = std::make_pair("Found", "");
	error[400] = std::make_pair("Bad Request", "errors/400.html");
	error[403] = std::make_pair("Forbidden", "errors/403.html");
	error[404] = std::make_pair("Not Found", "errors/404.html");
	error[405] = std::make_pair("Method Not Allowed", "errors/405.html");
	error[406] = std::make_pair("Not Acceptable", "errors/406.html");
	error[408] = std::make_pair("Request Timeout", "errors/408.html");
	error[411] = std::make_pair("Length Required", "errors/411.html");
	error[500] = std::make_pair("Internal Server Error", "errors/500.html");
	error[501] = std::make_pair("Not Implemented", "errors/501.html");
	error[504] = std::make_pair("Gateway Timeout", "errors/504.html");
	return (error);
}

//std::map<int, std::pair<std::string, std::string> > ServerConfig::_errorPages = ServerConfig::_initStatus();

// _____________  CONSTRUCTORS ______________________________________


ServerConfig::ServerConfig(): loc(true)
{
	_initKeys();
	_autoIndex = false;
	_root = "html";
	_maxBodySize = 10000000;
	_cgiConf[".sh"] = "/bin/bash";
	_cgiConf[".js"] = "/usr/bin/node";
	_cgiConf[".php"] = "/usr/bin/php";
	_cgiConf[".py"] = "/usr/bin/python3";
	_host = "localhost";
	_ip = "127.0.0.1";
	_port.push_back(8080);
	_allowedMethods.clear();
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	// _errorPages[403] = "./errors/403.html";
	// _errorPages[404] = "./errors/404.html";
	// _errorPages[500] = "./errors/500.html";
	//ERROR PAGES
}

ServerConfig::ServerConfig(std::string file): loc(true)
{
//	std::cout << "NEWSERV:" << "\n" << file << " ----------------------------------------------" << std::endl;
	_initKeys();
	_autoIndex = false;
	_root = ".";
	_maxBodySize = 10000000;
	_allowedMethods.clear();
	_port.clear();
	_serverName.clear();
	_locations.clear();
	_cgiConf[".sh"] = "/bin/bash";
	_cgiConf[".js"] = "/usr/bin/node";
	_cgiConf[".php"] = "/usr/bin/php";
	_cgiConf[".py"] = "/usr/bin/python3";
	_errorPages = _initStatus();
	// _errorPages[403] = "./errors/403.html";
	// _errorPages[404] = "./errors/404.html";
	// _errorPages[500] = "./errors/500.html";
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
//	copyMap(_errorPages, src._errorPages);
	_errorPages = src._errorPages;
	_maxBodySize = src._maxBodySize;
	_cgiConf = src._cgiConf;
	_autoIndex = src._autoIndex;
	_vars = src._vars;
	_keys = src._keys;
//	empty = src.empty;
	return (*this);
}

std::map<int, std::pair<std::string, std::string> > ServerConfig::operator=(const std::map<int, std::pair<std::string, std::string> > &val)
{
	std::map<int, std::pair<std::string, std::string> >	src = val;
	std::map<int, std::pair<std::string, std::string> >	ret;
	for (std::map<int, std::pair<std::string, std::string> >::iterator it = src.begin(); it != src.end(); it++)
		ret[it->first] = std::make_pair(it->second.first, it->second.second);
	return (ret);
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
	_keys.clear();
	_vars.clear();
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

const std::map<int, std::pair<std::string, std::string> > ServerConfig::getErrorPages() const
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
	if (Parse::ipCheck(host))
		setIp(host);
	else
		setHostName(host);
}

void ServerConfig::setHostName(const std::string& host)
{
		_host = host;
		_vars["host"] = true;
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
//	std::cout << _vars["port"] << "\n";
}

void ServerConfig::unsetPort(int port)
{
	std::vector<int>::iterator newEnd = std::remove(_port.begin(), _port.end(), port);
	_port.erase(newEnd, _port.end());
	// if (_port.empty())
	// 	return (1);
	// return (0);
//	std::cout << _vars["port"] << "\n";
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
	std::map<int, std::pair<std::string, std::string> >::iterator it = _errorPages.find(code);
	if (it == _errorPages.end())
		_errorPages[code] = std::make_pair("", page);
	else
		_errorPages[code] = std::make_pair(it->second.first, page);
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
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw std::invalid_argument("Error: unknown method in allowed_method: " + method);	
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

