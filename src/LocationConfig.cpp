/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:24:19 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/08 15:57:00 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/LocationConfig.hpp"

// _____________  CONSTRUCTORS ______________________________________

LocationConfig::LocationConfig(): loc(false) {}

LocationConfig::LocationConfig(std::string url, std::map<int, std::pair<std::string, std::string> > err, std::string file): loc(false), _uri(url)
{
	// std::cout << "UN LOC CONSTRUCT, URL: " + url + "\n";
	_autoIndex = false;
    _allowUpload = false;
	_root = "";
	_errorPages = err;
    _index = "index.html";
	_cgiConf[".sh"] = "/bin/bash";
	_cgiConf[".js"] = "/usr/bin/node";
	_cgiConf[".php"] = "/usr/bin/php";
	_cgiConf[".py"] = "/usr/bin/python3";
    _initKeys();
    Parse::complexParse<LocationConfig>(*this, file);
}

LocationConfig& LocationConfig::operator=(const LocationConfig& src)
{
    _uri = src._uri;
	_root = src._root;
    _return = src._return;
    _index = src._index;
	_allowedMethods = src._allowedMethods;
    _autoIndex = src._autoIndex;
    _uploadDir = src._uploadDir;
	_allowedMethods = src._allowedMethods;
	_errorPages = src._errorPages;
	_cgiConf = src._cgiConf;
	return (*this);
}

std::map<int, std::pair<std::string, std::string> > LocationConfig::operator=(const std::map<int, std::pair<std::string, std::string> > &val)
{
	std::map<int, std::pair<std::string, std::string> >	src = val;
	std::map<int, std::pair<std::string, std::string> >	ret;
	for (std::map<int, std::pair<std::string, std::string> >::iterator it = src.begin(); it != src.end(); it++)
		ret[it->first] = std::make_pair(it->second.first, it->second.second);
	return (ret);
}

LocationConfig::LocationConfig(const LocationConfig& src): loc(false)
{
	*this = src;
}

LocationConfig::~LocationConfig()
{
	_allowedMethods.clear();
    _errorPages.clear();
	_cgiConf.clear();
}

void LocationConfig::_initKeys()
{
	_keys["upload_dir"] = &Parse::uploadDirParse;
	_keys["return"] = &Parse::returnParse;
    _keys["root"] = &Parse::rootParse<LocationConfig>;
   	_keys["allow_upload"] = &Parse::allowUploadParse;
   	_keys["autoindex"] = &Parse::autoIndexParse<LocationConfig>;
    _keys["index"] = &Parse::indexParse;
    _keys["error_page"] = &Parse::errorParse<LocationConfig>;
    _keys["allow_methods"] = &Parse::allowMethodsParse<LocationConfig>;
 	_keys["cgi"] = &Parse::cgiParse<LocationConfig>;

    _vars["upload_dir"] = false;
	_vars["return"] = false;
    _vars["root"] = false;
   	_vars["allow_upload"] = false;
   	_vars["autoindex"] = false;
    _vars["index"] = false;
    _vars["error_page"] = false;
    _vars["allow_methods"] = false;
 	_vars["cgi"] = false;
}

// _____________  GETTERS ______________________________________

bool LocationConfig::getAutoIndex() const
{
    return (_autoIndex);
}
bool LocationConfig::getAllowUpload() const
{
    return (_allowUpload);
}

const std::string& 	LocationConfig::getUri() const
{
    return (_uri);
}

const std::string& 	LocationConfig::getRoot() const
{
    return (_root);
}

const std::string& 	LocationConfig::getReturn() const
{
    return (_return);
}

const std::string& 	LocationConfig::getIndex() const
{
    return (_index);
}

const std::string& 	LocationConfig::getUploadDir() const
{
    return (_uploadDir);
}

const std::map<std::string, std::string>& 		LocationConfig::getCgiConf() const
{
    return (_cgiConf);
}

const std::vector<std::string>& 	LocationConfig::getAllowedMethods() const
{
    return (_allowedMethods);
}

const std::map<int, std::pair<std::string, std::string> > 	LocationConfig::getErrorPages() const
{
    return (_errorPages);
}

const std::map<std::string, LocationConfig::func>& 	LocationConfig::getKeys() const
{
    return (_keys);
}

const std::map<std::string, bool>& 	LocationConfig::getVars()
{
    return (_vars);
}

// _____________  SETTERS ______________________________________

void LocationConfig::setRoot(const std::string& root)
{
	if (_vars["root"])
		throw std::invalid_argument("Error: \"root\" directive is duplicate");
	_root = root;
	_vars["root"] = true;	
}

void LocationConfig::setErrorPage(int code, const std::string& page)
{
	_errorPages[code] = std::make_pair("", page);
	if (!_vars["error_page"])
		_vars["error_page"] = true;
}

void LocationConfig::setCgiConf(const std::string &ext, const std::string &path)
{
	_cgiConf[ext] = path;
	if (!_vars["cgi"])
		_vars["cgi"] = true;
}

void LocationConfig::setAutoIndex(bool autoindex)
{
	if (_vars["autoindex"])
		throw std::invalid_argument("Error:\"autoindex\" directive is duplicate ");
	_autoIndex = autoindex;
	_vars["autoindex"] = true;
}

void 	LocationConfig::setAllowMethod(const std::string& method)
{
	for (std::vector<std::string>::iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); it++)
	{
		if (*it == method)
			throw std::invalid_argument("Error: method duplication in allowed_method");
	}
	_allowedMethods.push_back(method);
}

void LocationConfig::setUri(const std::string& uri)
{
    _uri = uri;
}

void LocationConfig::setReturn(const std::string& alias)
{
    
    if (_vars["return"])
		throw std::invalid_argument("Error:\"return\" directive is duplicate ");
    _return = alias;
    _vars["return"] = true;
}

void LocationConfig::setIndex(const std::string& index)
{
    if (_vars["index"])
		throw std::invalid_argument("Error:\"index\" directive is duplicate ");
    _index = index;
    _vars["index"] = true;
}

void LocationConfig::setAllowUpload(bool allowUpload)
{
    if (_vars["allow_upload"])
		throw std::invalid_argument("Error:\"allow_upload\" directive is duplicate ");

    _allowUpload = allowUpload;
    _vars["allow_upload"] = true;
}

void LocationConfig::setUploadDir(const std::string& uploadDir)
{
    if (_vars["upload_dir"])
		throw std::invalid_argument("Error:\"upload_dir\" directive is duplicate ");

    _uploadDir = uploadDir;
    _vars["upload_dir"] = true;
}

void LocationConfig::setVars(const std::string& key)
{
	_vars[key] = true;
}