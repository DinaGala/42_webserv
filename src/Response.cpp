#include "Response.hpp"

//////////////////////// STATIC ASSETS ////////////////////////////////////////

/*std::map<int, std::string> Response::initStatus()
{
	std::map<int, std::string> error;

	error[100] = "Continue";
	error[200] = "OK";
	error[201] = "Created";
	error[204] = "No Content";
	error[302] = "Found";
	error[400] = "Bad Request";
	error[403] = "Forbidden";
	error[404] = "Not Found";
	error[405] = "Method Not Allowed";
	error[406] = "Not Acceptable";
	error[408] = "Request Timeout";
	error[411] = "Length Required";
	error[500] = "Internal Server Error";
	error[501] = "Not Implemented";
	error[505] = "HTTP Version Not Supported";
	return (error);
}*/

std::map<int, std::pair<std::string, std::string> > Response::_initStatus()
{
	std::map<int, std::pair<std::string, std::string> > error;

	error[100] = std::make_pair("Continue", "");
	error[200] = std::make_pair("OK", "");
	error[201] = std::make_pair("Created", "");
	error[204] = std::make_pair("No Content", "");
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

std::map<int, std::pair<std::string, std::string> > Response::_status = Response::_initStatus();

//////////////////////////////////////////////////////////////////////////////

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

//////////////////////// SETTERS ///////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////

//parses Cgi's response: separates headers from body
void	Response::_parseCgiResponse(void)
{
	std::string::size_type	found = this->_response.find("\n\n");

	if (found == std::string::npos)
		found = this->_response.find("\r\n\r\n");
	if (found == std::string::npos)
	{
		this->_body = this->_response;
		this->_response = "";
		return ;
	}
	this->_body = this->_response.substr(found + 2, this->_response.size());
	this->_response = this->_response.substr(0, found) + "\n";
}

//writes and returns the server's response
std::string	&Response::getResponse(int code)
{
	if (this->_cgi_path.empty() == false) // if there's cgi
	{
		if (access(this->_cgi_path.c_str(), F_OK))
			return (this->sendError(404), this->_response);
		if (access(this->_cgi_path.c_str(), X_OK))
			return (this->sendError(403), this->_response);
		Cgi	cgi(8080, this->_method, this->_socket);
		cgi.setEnvVars(this->_cgi_path, "localhost", "serv_name");
		if (cgi.executeCgi(this->_response, this->_timeout))
			return (sendError(504), this->_response);
		this->_parseCgiResponse();
	}
	this->putGeneralHeaders();
	if (this->_body == "" && this->_path.empty() == false)
	{
		int error = this->fileToBody(this->_path);
		if (error)
			return (sendError(error), this->_response);
	}
	if (!this->_body.empty())
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
	this->_response.insert(0, this->putStatusLine(code));
	return (this->_response);
}

/////////////////////// PUT HEADERS (AND STATUS LINE) //////////////////////////

//puts status line in the response
std::string	Response::putStatusLine(int code)
{
	return ("HTTP/1.1 " + ft_itoa(code) + "\r\n");
}

//puts general header in the response: date, server, keep-alive and connection
void	Response::putGeneralHeaders(void)
{
	std::time_t	date = std::time(NULL);
	this->_response += "Date: ";
	this->_response += std::asctime(std::localtime(&date));
	this->_response += "Server: " + _servname + "\r\n";
	this->_response += "Keep-Alive: timeout=" + ft_itoa(this->_timeout);
	this->_response += ", max=" + ft_itoa(this->_maxconnect) + "\r\n";
	if (this->_connection)
		this->_response += "Connection: close\r\n";
	else
		this->_response += "Connection: keep-alive\r\n";
}

//checks mime type + adds specific POST headers in the response
bool	Response::putPostHeaders(const std::string &file)
{
	std::string	ext = file.substr(file.find_last_of("."));
	std::ifstream	mime("mime.types");
	std::string		line("");
	if (!mime.is_open())
		return (sendError(500), 1);
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
			this->_response += "Content-Type: text/plain\r\n";
		else
			this->_response += "Content-Type: application/octet-stream\r\n";
		return (0);
	}
	this->_response += "Content-Type: ";
	this->_response += line.substr(line.find_first_not_of("\t \n\v\r", ext.size())) + "\r\n";
	return (0);
}

///////////////////////////////////////////////////////////////////////////////

//puts file content in body string. If something's wrong, returns error code
int	Response::fileToBody(const std::string &path)
{
	if (access(path.c_str(), F_OK))//if given error page doesn't exist
		return (404);
	else if (access(path.c_str(), R_OK))//if server doesn't have the right to read
		return (403);
	std::ifstream	rdfile(path.c_str());
	if (!rdfile.is_open())
		return (500);
	std::ostringstream	content;
	content << rdfile.rdbuf();
	this->_body = content.str();
	return (0);
}

//makes error response. If there's an error creating the response,
// a severe internal server error page is sent (505)
void	Response::sendError(int code)
{
	this->_response = "Content-Type: text/html\r\n";
	int error = fileToBody(this->_status.at(code).second);
	if (error && fileToBody(this->_status.at(error).second))//true if we have a double error
	{
		this->_response += "Content-Length: 75\r\n";
		this->_response += "\n<html><body><h1>505</h1>"
							"<h2>Severe Internal Server Error</h2></body></html>";
		this->_response.insert(0, "HTTP/1.1 505 Severe Internal Server Error\r\n");
		return ;
	}
	this->_response.insert(0, this->putStatusLine(code));
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
}
