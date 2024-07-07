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

std::map<std::string, std::string> Response::_status = Response::initStatus();
///////

Response::Response(): _path("./html/test.html"), _servname("webserv"), _timeout(10000), _maxconnect(10), _connection(false), _code(0)
{}

Response::Response(const Response &r)
{
	this->_cgi_path = r._cgi_path;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_cgi_path = r._cgi_path;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
	return (*this);
}

void	Response::setBody(const std::string &msg)
{
	this->_body += msg;
}

void	Response::setCgiPath(const std::string &cgi)
{
	this->_cgi_path = cgi;
}

void	Response::setCode(const int &code)
{
	this->_code = code;
}

void	Response::setSocket(int sock)
{
	this->_socket = sock;
}

void	Response::setMethod(const std::string &meth)
{
	this->_method = meth;
}

std::string	Response::_toString(std::string::size_type n)
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

void	Response::_parseCgiResponse(void)
{
	std::string::size_type	found = this->_response.find("\n\n");

	if (found == std::string::npos)
	{
		this->_body = this->_response;
		this->_response = "";
		return ;
	}
	this->_body = this->_response.substr(found + 2, this->_response.size());
	this->_response = this->_response.substr(0, found);
}

//writes and returns the server's response
std::string	&Response::getResponse(const std::string &code)
{
	if (this->_cgi_path.empty() == false)
	{
		Cgi	cgi(8080, this->_method, this->_socket);
		cgi.setEnvVars(this->_cgi_path, "localhost", "serv_name");
		this->_response = cgi.executeCgi();
		this->_parseCgiResponse();
	}
	this->putGeneralHeaders();
	if (this->_body == "" && this->_path.empty() == false)
	{
		if (this->fileToBody(this->_path))
			return (this->_response);
	}
	if (!this->_body.empty())
		this->_response += "Content-Length: " + this->_toString(this->_body.size()) + "\n\n";
	this->_response += this->_body;
	this->_response.insert(0, this->putStatusLine(code));
	return (this->_response);
}

//puts status line in the response
std::string	Response::putStatusLine(const std::string &code)
{
	return ("HTTP/1.1 " + code + " " + this->_status.at(code) + "\r\n");
}

//puts general header in the response: date, server, keep-alive and connection
void	Response::putGeneralHeaders(void)
{
	std::time_t	date = std::time(NULL);
	this->_response += "Date: ";
	this->_response += std::asctime(std::localtime(&date));
	this->_response += "Server: " + _servname + "\n";
	this->_response += "Keep-Alive: timeout=" + this->_toString(this->_timeout);
	this->_response += ", max=" + this->_toString(this->_maxconnect) + "\n";
	if (this->_connection)
		this->_response += "Connection: close\n";
	else
		this->_response += "Connection: keep-alive\n";
}

//checks mime type + adds specific POST headers in the response
bool	Response::putPostHeaders(const std::string &file)
{
	std::string	ext = file.substr(file.find_last_of("."));
	std::ifstream	mime("mime.types");
	std::string		line = "";
	if (!mime.is_open())
		return (sendError("500", "errors/500.html"), 1);
	while (1)
	{
		if (mime.eof())
			break ;
		getline(mime, line);
		if (line.find(ext) != std::string::npos)
			break ;
	}
	this->_response += "Location: /"; //PROBABLY NOT THIS ONE
	if (line == "" || mime.eof())
	{
		if (access(file.c_str(), X_OK)) // if not executable
			this->_response += "Content-Type: text/plain\n";
		else
			this->_response += "Content-Type: application/octet-stream\n";
		return (0);
	}
	this->_response += "Content-Type: ";
	this->_response += line.substr(line.find_first_not_of("\t \n\v\r", ext.size())) + "\n";
	return (0);
}

//puts file content in body string. If something's wrong, sends error
bool	Response::fileToBody(const std::string &path)
{
	if (access(path.c_str(), F_OK))
		return (sendError("404", "errors/404.html"), 1);
	else if (access(path.c_str(), R_OK))
		return (sendError("403", "errors/403.html"), 1);
	std::ifstream	rdfile(path.c_str());
	if (!rdfile.is_open())
		return (sendError("500", "errors/500.html"), 1);
	std::ostringstream	content;
	content << rdfile.rdbuf();
	this->_body = content.str();
	return (0);
}

//makes error response. If there's an error with the error page,
// a severe internal server error page is sent
void	Response::sendError(const std::string &code, const std::string &path)
{
	this->_response = "Content-Type: text/html\n";
	if (fileToBody(path))
	{
		this->_response += "Content-Length: 75\n";
		this->_response += "\n<html><body><h1>505</h1>"
							"<h2>Severe Internal Server Error</h2></body></html>";
		this->_response.insert(0, "HTTP/1.1 505 Severe Internal Server Error\r\n");
		return ;
	}
	this->_response.insert(0, this->putStatusLine(code));
	this->_response += "Content-Length: " + this->_toString(this->_body.size()) + "\n\n";
	this->_response += this->_body;
}
