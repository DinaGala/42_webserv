/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:24:19 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/19 22:22:59 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}



LocationConfig::LocationConfig(std::stringstream &file)
{
	
}

LocationConfig& LocationConfig::operator=(const LocationConfig& src)
{
    _uri = src._uri;
	_root = src._root;
    _alias = src._alias;
    _index = src._index;
    _autoIndex = src._autoIndex;
    _allowUpload = src._allowUpload;
	_errorPages = src._errorPages;
	_cgiConf = src._cgiConf;
	return (*this);
}

LocationConfig::LocationConfig(const LocationConfig& src)
{
	*this = src;
}

LocationConfig::~LocationConfig()
{
	_allowedMethods.clear();
    _errorPages.clear();
	_cgiConf.clear();
}


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

const std::string& 	LocationConfig::getAlias() const
{
    return (_alias);
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

const std::map<int, std::string>& 	LocationConfig::getErrorPages() const
{
    return (_errorPages);
}

void LocationConfig::setRoot(const std::string& root)
{
	_root = root;
}

void LocationConfig::setErrorPage(int code, const std::string& page)
{
	_errorPages.insert(std::make_pair(code, page));
}

void LocationConfig::setCgiConf(const std::string &ext, const std::string &path)
{
	_cgiConf.insert(std::make_pair(ext, path));
}

void LocationConfig::setAutoIndex(bool autoindex)
{
	_autoIndex = autoindex;
}

void 	LocationConfig::setAllowedMethod(const std::string& method)
{
	_allowedMethods.push_back(method);
}

void LocationConfig::setUri(const std::string& uri)
{
    _uri = uri;
}

void LocationConfig::setAlias(const std::string& alias)
{
    _alias = alias;
}

void LocationConfig::setIndex(const std::string& index)
{
    _index = index;
}

void LocationConfig::setAllowUpload(bool allowUpload)
{
    _allowUpload = allowUpload;
}

void LocationConfig::setUploadDir(const std::string& uploadDir)
{
    _uploadDir = uploadDir;
}
