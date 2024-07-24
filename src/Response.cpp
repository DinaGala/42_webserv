#include "Response.hpp"

void	Response::cleanResponse()
{
	_body.clear();
	_reqbody.clear();
	_response.clear();
	_servname.clear();
	_method.clear();
	// ...
}

//////////////////////// STATIC ASSETS ////////////////////////////////////////

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

Response::Response(): _cgi(false), _keep_alive(true), _code(200), _req(NULL) {}


Response::Response(const Response &r): _req(r._req)
{
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
	this->_query = r._query;
	this->_reqbody = r._reqbody;
	this->_servname = r._servname;
	this->_method = r._method;
	this->_host = r._host;
	this->_socket = r._socket;
	this->_port = r._port;
	this->_keep_alive = r._keep_alive;
	this->_cgi = r._cgi;
	this->_cgiargs = r._cgiargs;
	this->_code = r._code;
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_body = r._body;
	this->_response = r._response;
	this->_query = r._query;
	this->_reqbody = r._reqbody;
	this->_servname = r._servname;
	this->_method = r._method; //tmp
	this->_host = r._host; //tmp
	this->_socket = r._socket; //tmp
	this->_port = r._port; //tmp
	this->_cgi = r._cgi; //tmp
	this->_keep_alive = r._keep_alive; //tmp
	this->_cgiargs = r._cgiargs;
	this->_code = r._code;
	this->_req = _req;

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

void	Response::setReq(const Request *rqt)
{
	this->_req = rqt;
}

std::vector<std::string>	Response::_setCgi(const std::string &path)
{
	std::vector<std::string>	args;
	if (access(path.c_str(), X_OK) == 0) //if executable
	{
		args.push_back("./" + path);
		this->_cgi = true;
		return (args);
	}
	std::string::size_type	found = path.find_last_of(".");
	std::map<std::string, std::string>	config = this->_req->getCgiConf();
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
std::string	&Response::makeResponse(const Request *req)
{
	if (!req)
		return (this->sendError(500), this->_response);
	else
		this->_req = req;
	if (this->_req->getCode() == 301)//redirect
	{
		this->_response = this->putStatusLine(301);
		this->putGeneralHeaders();
		this->_response += "Location: " + this->_req->getPath() + "\r\n";
		return (this->_response);
	}
	else if (this->_req->getCode() > 301)
		return (this->sendError(this->_req->getCode()), this->_response);
	std::string	method = this->_req->getMethod();

	std::cout << "\033[33;1mMETHOD: " << method << "\033[0m" << std::endl;
	//std::cout << "\033[33;1mPATH: " << this->_req->getPath() << "\033[0m" << std::endl;
	std::cout << "\033[33;1mREQ PATH: " << this->_req->getPath() << "\033[0m" << std::endl;
	//std::cout << "\033[33;1mREQ CODE: " << this->_req->getCode() << "\033[0m" << std::endl;

	if (this->_req->getCgi())
		this->_cgiargs = this->_setCgi(this->_req->getPath());
	if (method == "GET")
		this->_handleGet();
	else if (method == "POST")
		this->_handlePost();
	else if (method == "DELETE")
		this->_handleDelete();
	std::cout << "\033[33;1mRESPONSE: DONE\033[0m" << std::endl;
	return (this->_response);
}

////////////////////// HANDLE REQUESTS BY METHOD ////////////////////////////

void	Response::_handleFavIcon()
{
	std::ifstream	icon("./assets/favicon_general.png");
	std::ostringstream	favicon;

	if (!icon.is_open())
		this->_body = "";
	else
	{
		favicon << icon.rdbuf();
		this->_body = favicon.str();
	}
	this->_response = this->putStatusLine(200);
	this->putGeneralHeaders();
	this->_response += "Content-Type: image/png\n\n";
	this->_response += this->_body;
}

void	Response::_handleGet()
{
	int	is_dir;

	//std::cout << "\033[32;1mhandle GET\033[0m" << std::endl;
	//std::cout << "\033[32;1mGET path \033[0m" << this->_req->getPath() << std::endl;
	if (this->_req->getPath() == "./favicon.ico")
	{
		std::cout << "\033[32;1mhandle favicon\033[0m" << std::endl;
		this->_handleFavIcon();
		return ;
	}
	is_dir = this->_isDir(this->_req->getPath());
	if (is_dir == -1)
	{
		this->sendError(500);
		return ;
	}
	else if (is_dir)
	{
		if (this->_req->getIndex() != "")
		{
			std::cout << "\033[32;1mGET not index\033[0m" << std::endl;
			this->_response = this->putStatusLine(200);
			this->putGeneralHeaders();
			int code = this->fileToBody(this->_req->getPath() + this->_req->getIndex());
			if (code)
			{
				this->sendError(code);
				return ;
			}
			this->_response += "\n\n" + this->_body;
			return ;
		}
		//else if (this->_req->getAutoIndex() == true)
		else if (this->_req->getAutoIndex() == false)// TMP
		{
			std::cout << "\033[32;1mGET autoindex\033[0m" << std::endl;
			this->_makeAutoIndex();
			return ;
		}
		else
		{
			this->sendError(403);
			return ;
		}
	}
	if (this->_cgi == true) // if there's cgi
	{
		if (access(this->_req->getPath().c_str(), F_OK)) // if cgi exists = 0
		{
			this->sendError(404);
			return ;
		}
		else if (access(this->_req->getPath().c_str(), X_OK))
		{
			this->sendError(403);
			return ;
		}
		//std::cout << "\033[1;34mGET: path " << this->_req->getPath() << "\033[0m" << std::endl;
		Cgi	cgi(this->_socket, *(this->_req));
		this->_cgiargs = this->_setCgi(this->_req->getPath());
		cgi.setEnvVars(this->_req->getPath(), this->_host, this->_servname, this->_query, this->_cgiargs);
		int	cgi_status = cgi.executeCgi(this->_response, TIMEOUT); // execute cgi
		std::cout << "GET->CGI->cgi_status: " << cgi_status << std::endl;
		if (cgi_status) // if cgi returns status != 0 -> error
		{
			this->sendError(cgi_status);
			return ;
		}
		this->_parseCgiResponse();
	}
	else //if not cgi
	{
		std::cout << "\033[1;31mGET: fileToBody\033[0m" << std::endl;
		int error = this->fileToBody(this->_req->getPath());
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
	if (access(this->_req->getPath().c_str(), F_OK))// file not found
	{
		this->sendError(404);
		return ;
	}
	if (access(this->_req->getPath().c_str(), W_OK))// no permissions
	{
		this->sendError(403);
		return ;
	}
	if (std::remove(this->_req->getPath().c_str()))
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
	filename.insert(0, this->_req->getUploadDir());
	if (stat("uploaded", &is_dir))
			return (this->sendError(500), 1);
	if (!S_ISDIR(is_dir.st_mode))// if it doesn't exist
		std::system("mkdir uploaded");
	//std::ofstream	newfile(this->_req->getFilename());
	std::ofstream	newfile(filename.c_str());
	if (!newfile.is_open())// creating/opening file failed
			return (this->sendError(500), 1);
	//newfile << this->_req->body;
	newfile << this->_body;
	return (0);
}

void	Response::_handlePost()
{
	std::cout << "\033[1;31mHandle Post fileName: " << this->_req->getFileName() << "\033[0m" << std::endl;
	if (this->_req->getFileName() != "")
	{
		if (this->_req->getAllowUpload())
		{
			this->sendError(403);
			return ;
		}
		if (this->_createFile())
			return ;
		this->_response = this->putStatusLine(201);
		this->putGeneralHeaders();
		//this->putPostHeaders(this->_req->getFileName());
		this->putPostHeaders("new_file.tmp");
	}
	else
	{
		this->_response = this->putStatusLine(200);
		this->putGeneralHeaders();
		this->_response += "\n\n<html><body>Form submitted!</body></html>";
	}
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
	const std::multimap<std::string, std::string> mp = this->_req->getAcceptedContent();
	std::multimap<std::string, std::string>::const_iterator it = mp.find(mime.substr(0, found));
	if (it != mp.end() && (it->second == mime.substr(found + 1) || mime.substr(found + 1) == "*"))
		return (0);
	return (1);
}

//if index.html
//	show index.html
//else if autoindex on
//	show listing
//else
//	show 403

int	Response::_isDir(const std::string &path) const
{
	struct stat	status;

	if (stat(path.c_str(), &status))
		return (-1);
	if (S_ISDIR(status.st_mode))
		return (1);
	return (0);
}

void	Response::_makeAutoIndex(void)
{
	struct dirent	*dp;
	DIR				*dir;
	int				is_dir;
	std::string		filename;

	if (this->_isNotAccepted("text/html"))
	{
		this->sendError(403);
		return ;
	}
	dir = opendir(this->_req->getPath().c_str());
	if (!dir)
	{
		//std::cout << "\033[31;1mmake AutoIndex error dir\033[0m" << std::endl;
		this->sendError(500);
		return ;
	}
	this->_body = "<html><head><title>INDEX</title><h1>Index of ";
	this->_body += this->_req->getPath() + "</h1></head><body>";
	while ((dp = readdir(dir)) != NULL)
	{
		filename = this->_req->getPath();
		if (dp->d_name[0] == '.')
			continue ;
		if (filename[filename.size() - 1] != '/')
			filename += "/";
		filename += dp->d_name;
		is_dir = this->_isDir(filename);
		//std::cout << "\033[33;1mmake AutoIndex:\n\t filename" << filename
		//		<< "\n\tpath: " << this->_req->getPath() << "\n\td_name: "
		//		<< dp->d_name<< "\033[0m" << std::endl;
		if (is_dir == -1)
		{
			std::cout << "\033[31;1mAutoIdx error while: " << filename << "\033[0m" << std::endl;
			this->sendError(500);
			closedir(dir);
			return ;
		}
		if (!access(filename.c_str(), X_OK) && !is_dir)
			continue ;
		if (filename == "./conf" || filename == "./errors" || filename == "./cgi-bin")
			continue ;
		//std::cout << "\033[31;1mmake AutoIndex file: " << filename << "\033[0m" << std::endl;
		this->_body += "<p><a href= ";
		this->_body += filename.substr(1);
		this->_body += ">";
		this->_body += dp->d_name;
		this->_body += "</a></p>\n";
	}
	this->_body += "</body></html>";
	this->_response = this->putStatusLine(200);
	this->putGeneralHeaders();
	this->_response += "\n\n" + this->_body;
	closedir(dir);
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
	//this->_response += "Location: " + this->_req->getUploadDir();
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
	if (this->_isNotAccepted("text/html"))
	{
		this->_response = this->putStatusLine(code);
		this->putGeneralHeaders();
		this->_response += "Content-Length: " + ft_itoa(this->_status.at(code).first.size()) + "\r\n\n";
		this->_response += ft_itoa(code) + this->_status.at(code).first;
		return ;
	}
	int error = fileToBody(this->_status.at(code).second);
	if (error && fileToBody(this->_status.at(error).second))//true if we have a double error
	{
		this->_response += "Content-Length: 22\r\n\r\n";
		this->_response += "Severe Internal Error\n";
		this->_response.insert(0, "HTTP/1.1 505 Severe Internal Server Error\r\n");
		return ;
	}
	this->_response = "Content-Type: text/html\r\n";
	this->_response.insert(0, this->putStatusLine(code));
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\n\n";
	this->_response += this->_body;
}

int	Response::getCode(void) const
{
	return (this->_code);
}

const std::string	&Response::getResponse(void) const
{
	return (this->_response);
}
