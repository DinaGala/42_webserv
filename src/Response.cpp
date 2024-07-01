#include "Response.hpp"

////// Static Assets

std::map<std::string, std::string> Response::initStatus()
{
	std::map<std::string, std::string> error;

	error["100"] = "Continue";
	error["200"] = "OK";
	error["201"] = "Created";
	error["204"] = "No Content";
	error["302"] = "Found";
	error["400"] = "Bad Request";
	error["403"] = "Forbidden";
	error["404"] = "Not Found";
	error["405"] = "Method Not Allowed";
	error["406"] = "Not Acceptable";
	error["408"] = "Request Timeout";
	error["411"] = "Length Required";
	error["500"] = "Internal Server Error";
	error["501"] = "Not Implemented";
	error["505"] = "HTTP Version Not Supported";
	return (error);
}

const std::map<std::string, std::string> Response::_status = Response::initStatus();
///////

Response::Response(): _cgi(""), _response(""), _body(""), _code(0), _servname("webserv")
{}

Response::Response(const Response &r)
{
	this->_cgi = r._cgi;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_cgi = r._cgi;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
	return (*this);
}

void	Response::setBody(const std::string &msg)
{
	this->_body += msg;
}

void	Response::setCgi(const std::string &cgi)
{
	this->_cgi = cgi;
}

std::string	Response::_itoa(std::string::size_type n)
{
	std::string	str;

	if (n == 0)
		return ("0");
	while (n)
	{
		str.insert(0, 1, n % 10 + '0');
		n = n / 10;
	}
	return (str);
}

std::string	&Response::getResponse(const std::string &code)
{
	if (this->_cgi.empty() && this->_cgi.find("HTTP/") != std::string::npos)
		return (this->_cgi);
	this->putStatusLine(code);
	if (this->_body.size() > 0)
	{
		this->_response += "Content-Length: " + this->_itoa(this->_body.size()) + "\n";
		this->_response += this->_body;
	}
	return (this->_response);
}

void	Response::putStatusLine(const std::string &code)
{
	this->_response = "HTTP/1.1 " + code + " ";
	this->_response += this->_status.at(code) + "\r\n";
}
void	Response::putGeneralHeaders(void)
{
	std::time_t	date = std::time(NULL);
	this->_response += "Date: ";
	this->_response += std::asctime(std::localtime(&date));
	this->_response += "Server: " + _servname;
}
