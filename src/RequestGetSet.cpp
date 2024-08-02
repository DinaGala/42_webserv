#include "Request.hpp"

// _____________  GETTERS _____________ 

const std::string& Request::getBuffer() const 
{
	return (_buffer);
}

Server&  Request::getServer() const 
{
	return (_server);
}

int  Request::getPort() const 
{
	return (_port);
}

int	Request::getStatus() const {
	return (_status);
}

const std::vector<std::string>& Request::getRequesLine() const
{
	return (_requestLine);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
	return (_headers);
}

const std::string& Request::getBody() const 
{
	return (_body);
}

const std::string& Request::getQuery() const 
{
	return (_query);
}

const std::string&  Request::getPath() const 
{
    return (_path);
}

const std::string& 	Request::getRoot() const
{
    return (_root);
}

bool Request::getRootLoc() const 
{
	return (_rootLoc);
}

const std::string&	Request::getMethod() const 
{
	return (_method);
}

int	Request::getCode() const 
{
	return (_code);
}

int Request::getPosLocation() const 
{
	return (_posLocation);
}

size_t	Request::getMaxBodySize() const 
{
	return (_maxBodySize);
}

const std::string& Request::getHost() const 
{
	return (_host);
}

const std::vector<std::string>& Request::getAllowedMethods() const
{
	return (_allowedMethods);
}

const std::map<int, std::pair<std::string, std::string> >& Request::getErrorPages() const 
{
	return (_errorPages);
}

const std::string& Request::getIndex() const 
{
	return (_index);
}

bool Request::getAutoIndex() const 
{
	return (_autoIndex);
}

bool Request::getAllowUpload() const 
{
	return (_allowUpload);
}

const std::string& 	Request::getUploadDir() const 
{
	return (_uploadDir);
}

const std::string& 	Request::getReturn() const 
{
	return (_return);
}

bool Request::getCgi() const
{
	return (_cgi);
}

const std::map<std::string, std::string>&  Request::getCgiConf() const 
{
	return (_cgiConf);
}

const std::vector<std::string>& Request::getServerNames() const
{
	return (_serverNames);
}

bool Request::getConnectionKeepAlive() const 
{
	return (_connectionKeepAlive);
}

const std::multimap<std::string, std::string>&	Request::getAcceptedContent() const 
{
	return (_acceptedContent);
}

const std::string&  Request::getBoundary() const 
{
	return (_boundary);
}

const std::map<std::string, std::string>& Request::getMultipartHeaders() const
{
	return (_multipartHeaders);
}

const std::string&	Request::getFileName() const 
{
	return (_fileName);
}

int Request::getContentLenght() const {
	return (_contentLenght);
}

// _____________  SETTERS _____________ 

void Request::setErrorPages(const std::map<int, std::pair<std::string, std::string> >&  errorPages) 
{
	copyMap(_errorPages, errorPages);
}

void Request::setIndex(const std::string& index) 
{
	_index = index;
}

void Request::setAutoIndex(bool autoindex) 
{
	_autoIndex = autoindex;
}

void Request::setAllowUpload(bool allowUpload) 
{
	_allowUpload = allowUpload;
}

void Request::setUploadDir(const std::string& uploadDir) 
{
	_uploadDir = uploadDir;
}

void Request::setReturn(const std::string& alias) 
{
	_return = alias;
}

void Request::setCgiConf(const std::map<std::string, std::string>& cgiConf) 
{
	_cgiConf = cgiConf;
}
