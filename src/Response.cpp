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

std::map<int, std::pair<std::string, std::string> > Response::_status = Response::_initStatus();

//////////////////////////////////////////////////////////////////////////////

Response::Response() 
{
	this->_method = "POST";
	this->_path = "cgi-bin/test.py";
	this->_path = "./cgi-bin/form_post.py"; // not found
	this->_path = "http://localhost:8080/cgi-bin/post_req.sh";
	this->_path = "./cgi-bin/post_test.js";
	this->_path = "./html/form.html";
	this->_servname = "webserv";
	this->_timeout = 10;
	this->_maxconnect = 10;
	this->_connection = false;
	this->_code = 0;
	this->_host = "localhost:8080";
	this->_port = 8080;
	this->_cgi = true;
	this->_cgi = false;
}

Response::Response(const Response &r)
{
	this->_path = r._path;
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_path = r._path;
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
	this->_path = cgi;
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
/*
void	Response::setStatus(const std::map<int, std::pair<std::string, std::string> > &status)
{
	this->_status = &status;
}*/
///////////////////////////////////////////////////////////////////////////

std::string	Response::_parseUrl(const std::string &url)
{
	std::string::size_type	found = url.find(this->_host);
	std::string::size_type	next;
	std::string str;

	if (found != std::string::npos)
		found += this->_host.size() + 1;
	else
		found = 0;
	next = url.find("?", found);
	if (next != std::string::npos)
	{
		this->_reqbody = url.substr(next, url.size());
		str = url.substr(found, next);
	}
	else
		str = url.substr(found);
	return (str);
}

//parses Cgi's response: separates headers from body
void	Response::_parseCgiResponse(void)
{
	std::string::size_type	found = this->_response.find("\n\n");

	if (found == std::string::npos)// if not \n\n
		found = this->_response.find("\r\n\r\n");
	if (found == std::string::npos)// if not \r\n\r\n => cgi's response is full body
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
	this->_path = this->_parseUrl(this->_path);
	if (this->_method == "GET")
		this->handleGet();
	else if (this->_method == "POST")
		this->_handlePost();
	else if (this->_method == "DELETE")
		this->_handleDelete();
	/*if (this->_cgi) // if there's cgi
	{
		if (access(this->_path.c_str(), F_OK)) // if cgi exists
			return (this->sendError(404), this->_response);
		if (access(this->_path.c_str(), X_OK)) // if cgi is executable
			return (this->sendError(403), this->_response);
		Cgi	cgi(this->_port, this->_method, this->_socket);
		cgi.setEnvVars(this->_path, this->_host, this->_servname);
		int	cgi_status = cgi.executeCgi(this->_response, this->_timeout); // execute cgi
		std::cout << "\033[1;32mgetResponse: cgi status"<< cgi_status << "\033[0m" << std::endl;
		if (cgi_status) // if cgi returns status != 0 -> error
			return (this->sendError(cgi_status), this->_response);
		this->_parseCgiResponse();
	}
	this->putGeneralHeaders();
	if (this->_body == "" && !this->_path.empty())// if no body but path
	{
		std::cout << "getResponse: path " << this->_path << std::endl;
		std::cout << "getResponse: body " << this->_body << std::endl;
		int error = this->fileToBody(this->_path);
		if (error)
			return (this->sendError(error), this->_response);
	}
	if (!this->_body.empty())// if body
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;// add body to response
	this->_response.insert(0, this->putStatusLine(code));// put status line*/
	return (this->_response);
}

////////////////////// HANDLE REQUESTS BY METHOD ////////////////////////////

void	Response::_handleGet()
{
	if (this->_cgi) // if there's cgi
	{
		if (access(this->_path.c_str(), F_OK)) // if cgi exists
			return (this->sendError(404), this->_response);
		if (access(this->_path.c_str(), X_OK)) // if cgi is executable
			return (this->sendError(403), this->_response);
		Cgi	cgi(this->_port, this->_method, this->_socket);
		cgi.setEnvVars(this->_path, this->_host, this->_servname);
		int	cgi_status = cgi.executeCgi(this->_response, this->_timeout); // execute cgi
		if (cgi_status) // if cgi returns status != 0 -> error
			return (this->sendError(cgi_status), this->_response);
		this->_parseCgiResponse();
	}
	else //if not cgi
	{
		int error = this->fileToBody(this->_path);
		if (error)
			return (this->sendError(error), this->_response);
	}
	this->putGeneralHeaders();
	if (!this->_body.empty())// if body
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;// add body to response
	this->_response.insert(0, this->putStatusLine(code));// put status line
}

void	Response::_handleDelete()
{
	if (access(this->_path.c_str(), F_OK))// file not found
	{
		this->sendError(404);
		return ;
	}
	if (access(this->_path.c_str(), W_OK))// no permissions
	{
		this->sendError(403);
		return ;
	}
	if (std::remove(this->_path.c_str()))
	{
		this->sendError(500);
		return ;
	}
	this->putStatusLine(204); // Success + No Content
	this->putGeneralHeaders;
}

//>1. GET form
//< return html form
//>2. POST form(?) + body
//< return No Content + html form / No Content + html submission confirmation

void	Response::_handlePost()
{
	if (access(this->_path.c_str(), F_OK)) // if file exists = 0 (upload?)
	{
		std::ofstream	newfile(this->_path);
		newfile << this->_reqbody << std::endl;
		return ;
	}
	if (access(this->_path.c_str(), X_OK)) // if file is executable = 0
	{
		int error = fileToBody(this->_path);
		if (error)
			sendError(error);
		else
		{
			this->putGeneralHeaders();
			this->_response.insert(0, this->putStatusLine(200));// put status line
			this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
			this->_response += this->_body;
		}
		return ;
	}
	Cgi	post(this->_port, this->_method, this->_socket);
	post.setEnvVars(this->_path, this->_host, this->_servname);
	int	cgi_status = post.executeCgi(this->_response, this->_timeout); // execute cgi
	if (cgi_status)
		this->sendError(cgi_status);
}

////////////////////////////////////////////////////////////////////////////////

/////////////////////// PUT HEADERS (AND STATUS LINE) //////////////////////////

//puts status line in the response
std::string	Response::putStatusLine(int code)
{
	return ("HTTP/1.1 " + ft_itoa(code) + " " + this->_status[code].first + "\r\n");
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
	if (this->_status.find(code) == this->_status.end())
		code = 500;
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
