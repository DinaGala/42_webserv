#include "Cluster.hpp"

std::map<std::string, std::string> Response::_status;

Response::Response()
{
	this->_status["100"] = "Continue";
	this->_status["200"] = "OK";
	this->_status["201"] = "Created";
	this->_status["204"] = "No Content";
	this->_status["302"] = "Found";
	this->_status["400"] = "Bad Request";
	this->_status["403"] = "Forbidden";
	this->_status["404"] = "Not Found";
	this->_status["405"] = "Method Not Allowed";
	this->_status["406"] = "Not Acceptable";
	this->_status["408"] = "Request Timeout";
	this->_status["411"] = "Length Required";
	this->_status["500"] = "Internal Server Error";
	this->_status["501"] = "Not Implemented";
	this->_status["505"] = "HTTP Version Not Supported";
}

Response::Response(const Response &r)
{
	(void)r;
	this->_status["100"] = "Continue";
	this->_status["200"] = "OK";
	this->_status["201"] = "Created";
	this->_status["204"] = "No Content";
	this->_status["302"] = "Found";
	this->_status["400"] = "Bad Request";
	this->_status["403"] = "Forbidden";
	this->_status["404"] = "Not Found";
	this->_status["405"] = "Method Not Allowed";
	this->_status["406"] = "Not Acceptable";
	this->_status["408"] = "Request Timeout";
	this->_status["411"] = "Length Required";
	this->_status["500"] = "Internal Server Error";
	this->_status["501"] = "Not Implemented";
	this->_status["505"] = "HTTP Version Not Supported";
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	(void)r;
	return (*this);
}

void	Response::setBody(const std::string &msg)
{
	this->_body += msg;
}

std::string	Response::_ft_itoa(unsigned int n)
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
	this->_response = "HTTP/1.1 " + code + " " + this->_status[code] + "\n";
	if (this->_body.size() > 0)
	{
		this->_response += "Content-Length: " + this->_ft_itoa(this->_body.size()) + "\n";
		this->_response += this->_body;
	}
	return (this->_response);
}