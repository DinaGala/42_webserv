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

Response::Response(Request &req): _code(200), _req(req) 
{
	//this->_req.getMethod() = "POST";
	//this->_req.getPath() = "cgi-bin/test.py";
	//this->_req.getPath() = "./cgi-bin/form_post.py"; // not found
	//this->_req.getPath() = "http://localhost:8080/cgi-bin/post_req.sh";
	//this->_req.getPath() = "./cgi-bin/post_test.js";
	//this->_req.getPath() = "./html/form.html";
	this->_servname = "webserv";
	//this->_timeout = 10;
	//this->_maxconnect = 10;
	this->_keep_alive = true;
	this->_host = "localhost:8080";
	this->_port = 8080;
	this->_cgi = false;
	this->_reqbody = "This is a temporary variable";
}

Response::Response(const Response &r): _req(r._req)
{
	//this->_req.setPath(r._req.getPath());
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	(void)r;
	return (*this);
}

//////////////////////// SETTERS ///////////////////////////////////////////
void	Response::setBody(const std::string &msg)
{
	this->_body += msg;
}

void	Response::setCode(const int &code)
{
	this->_code = code;
}

std::vector<std::string>	Response::_setCgi(std::string &path)
{
	std::vector<std::string>	args;
	if (access(path.c_str(), X_OK) == 0) //if executable
	{
		args.push_back("./" + path);
		this->_cgi = true;
		return (args);
	}
	std::string::size_type	found = path.find_last_of(".");
	std::map<std::string, std::string>	config = this->_req.getCgiConf();
	std::string	ext;
	if (found != std::string::npos)// if there's extension
	ext = path.substr(found);
	if (config.find(ext) != config.end()) //if it's not allowed
	{
		this->_cgi = true;
		args.push_back(config[ext]);
	}
	args.push_back(path);
	return (args);
}
///////////////////////////////////////////////////////////////////////////

std::string	Response::_parseUrl(const std::string &url)
{
	std::string::size_type	found = 0;
	std::string::size_type	query;
	std::string str;

	query = url.find("?", found);
	if (query != std::string::npos) // if there's a ?
	{
		this->_query = url.substr(query + 1, url.size());
		str = url.substr(found, query);
	}
	else
		str = url;
	str.insert(0, ".");
	if (access(str.c_str(), X_OK) == 0)
		this->_cgi = true;
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
	//if (this->_req.getCode() == 301)
	this->_path = this->_parseUrl(this->_req.getPath());
	this->_cgiargs = this->_setCgi(this->_path);
	if (code == 301)
	{
		this->putStatusLine(301);
		this->putGeneralHeaders();
		return (this->_response);
	}
	else if (code > 301)
		return (this->sendError(code), this->_response);
	std::cout << "\033[33;1mMETHOD: " << this->_req.getMethod() << "\033[0m" << std::endl;
	std::cout << "\033[33;1mPATH: " << this->_path << "\033[0m" << std::endl;
	if (this->_req.getMethod() == "GET")
		this->_handleGet();
	else if (this->_req.getMethod() == "POST")
		this->_handlePost();
	else if (this->_req.getMethod() == "DELETE")
		this->_handleDelete();
	return (this->_response);
}

////////////////////// HANDLE REQUESTS BY METHOD ////////////////////////////

void	Response::_handleGet()
{
	if (this->_cgi) // if there's cgi
	{
		if (access(this->_path.c_str(), F_OK)) // if cgi exists = 0
		{
			this->sendError(404);
			return ;
		}
		std::cout << "\033[1;34mGET: path " << this->_path << "\033[0m" << std::endl;
		Cgi	cgi(this->_socket, this->_req);
		cgi.setEnvVars(this->_path, this->_host, this->_servname, this->_query, this->_cgiargs);
		int	cgi_status = cgi.executeCgi(this->_response, TIMEOUT); // execute cgi
		if (cgi_status) // if cgi returns status != 0 -> error
		{
			this->sendError(cgi_status);
			return ;
		}
		this->_parseCgiResponse();
	}
	else //if not cgi
	{
		int error = this->fileToBody(this->_path);
		if (error)
		{
			this->sendError(error);
			return ;
		}
	}
	this->putGeneralHeaders();
	if (!this->_body.empty())// if body
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;// add body to response
	this->_response.insert(0, this->putStatusLine(this->_code));// put status line
}

void	Response::_handleDelete()
{
	if (access(this->_req.getPath().c_str(), F_OK))// file not found
	{
		this->sendError(404);
		return ;
	}
	if (access(this->_req.getPath().c_str(), W_OK))// no permissions
	{
		this->sendError(403);
		return ;
	}
	if (std::remove(this->_req.getPath().c_str()))
	{
		this->sendError(500);
		return ;
	}
	this->putStatusLine(204); // Success + No Content
	this->putGeneralHeaders();
}

//>1. GET form
//< return html form
//>2. POST form(?) + body
//< return No Content + html form / No Content + html submission confirmation

bool	Response::_createFile(void)
{
	struct stat	is_dir;
	std::string	filename = "new_file.tmp";
	filename.insert(0, this->_req.getUploadDir());
	if (stat("uploaded", &is_dir))
			return (this->sendError(500), 1);
	if (!S_ISDIR(is_dir.st_mode))// if it doesn't exist
		std::system("mkdir uploaded");
	//std::ofstream	newfile(this->_req.getFilename());
	std::ofstream	newfile(filename.c_str());
	if (!newfile.is_open())// creating/opening file failed
			return (this->sendError(500), 1);
	//newfile << this->_req.body;
	newfile << this->_body;
	return (0);
}

void	Response::_handlePost()
{
	if (this->_req.getPath() == "/submit-form")
	{
		this->_response = this->putStatusLine(200);
		this->putGeneralHeaders();
		this->_response += "\n\n<html><body>Form submitted!</body></html>";
	}
	else if (this->_req.getPath() == "/upload")
	{
		if (this->_req.getAllowUpload())
		{
			this->sendError(403);
			return ;
		}
		if (this->_createFile())
			return ;
		this->_response = this->putStatusLine(201);
		this->putGeneralHeaders();
		this->putPostHeaders("new_file.tmp");
		//this->putPostHeaders(this->_req.getFilename());
	}
	else
		this->sendError(501);
}

////////////////////////////////////////////////////////////////////////////////

//checks if it's all accepted (*/*). If it's not, it checks if type is accepted
//(in text/html, text is type and html is subtype). If type is accepted, checks if
//subtype is accepted or it's *
bool	Response::_isNotAccepted(std::string mime)
{
	if (mime == "*/*")
		return (0);
	std::string::size_type	found = mime.find("/");
	if (found == std::string::npos)
		return (1);
	const std::multimap<std::string, std::string> mp = this->_req.getAcceptedContent();
	std::multimap<std::string, std::string>::const_iterator it = mp.find(mime.substr(0, found));
	if (it != mp.end() && (it->second == mime.substr(found + 1) || mime.substr(found + 1) == "*"))
		return (0);
	return (1);
}

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
	if (!this->_keep_alive)
		this->_response += "Connection: close\r\n";
	else
	{
		this->_response += "Connection: keep-alive\r\n";
		this->_response += "Keep-Alive: timeout=" + ft_itoa(TIMEOUT);
		this->_response += ", max=" + ft_itoa(MAXCONNECT) + "\r\n";
	}
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
	//this->_response += "Location: " + this->_req.getUploadDir();
	this->_response += "Location: /"; //TMP
	if (line == "" || mime.eof())
	{
		if (access(file.c_str(), X_OK)) // if not executable
		{
			if (this->_isNotAccepted("text/plain"))
				return (this->sendError(406), 1);
			else
				this->_response += "Content-Type: text/plain\r\n";
		}
		else
		{
			if (this->_isNotAccepted("application/octet-stream"))
				return (this->sendError(406), 1);
			else
				this->_response += "Content-Type: application/octet-stream\r\n";
		}
		return (0);
	}
	line = line.substr(line.find_first_not_of("\t \n\v\r", ext.size()));
	if (this->_isNotAccepted(line))
		return (this->sendError(406), 1);
	this->_response += "Content-Type: " + line + "\r\n";
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
	if (this->_status.find(code) == this->_status.end())
		code = 500;
	int error = fileToBody(this->_status.at(code).second);
	if (error && fileToBody(this->_status.at(error).second))//true if we have a double error
	{
		this->_response += "Content-Length: 22\r\n\r\n";
		//this->_response += "\n<html><body><h1>505</h1>"
		//					"<h2>Severe Internal Server Error</h2></body></html>";
		this->_response += "Severe Internal Error\n";
		this->_response.insert(0, "HTTP/1.1 505 Severe Internal Server Error\r\n");
		return ;
	}
	this->_response = "Content-Type: text/html\r\n";
	this->_response.insert(0, this->putStatusLine(code));
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
}
