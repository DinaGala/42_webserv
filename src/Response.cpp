#include "Response.hpp"

const std::set<std::string> Response::_initSensitive(void)
{
	std::set<std::string> sen;

	sen.insert("./conf");
	sen.insert("./errors");
	sen.insert("./src");
	sen.insert("./inc");
	sen.insert("./assets");
	sen.insert("./mime.types");
	sen.insert("./assets");
	return (sen);
}

const std::set<std::string> Response::_sensitive = _initSensitive();

void	Response::cleanResponse()
{
	_errorPages.clear();
	_cgiargs.clear();
	_body.clear();
	_response.clear();
	_code = 200;
	_req = NULL;
}

Response::Response(): _code(200), _req(NULL) {}

Response::Response(const Response &r): _req(r._req)
{
	this->_response = r._response;
	this->_body = r._body;
	this->_code = r._code;
	this->_cgiargs = r._cgiargs;
	if (r._req)
		this->_errorPages = r._req->getErrorPages();
}

Response::~Response() {}

Response	&Response::operator=(const Response &r)
{
	this->_body = r._body;
	this->_req = _req;
	this->_response = r._response;
	this->_cgiargs = r._cgiargs;
	this->_code = r._code;
	if (r._req)
		this->_errorPages = r._req->getErrorPages();
	return (*this);
}

/*
Reads URL and returns a vector with:
./binay (if it's executable)
or
(0) interpreter (p.e. /bin/bash)
(1) cgi (p.e. script.sh)
*/
std::vector<std::string>	Response::_findCgiArgs(const std::string &path)
{
	std::vector<std::string>	args;
	if (access(path.c_str(), X_OK) == 0) //if executable
	{
		args.push_back("./" + path);
		return (args);
	}
	std::string::size_type	found = path.find_last_of(".");
	std::map<std::string, std::string>	config = this->_req->getCgiConf();
	std::string	ext;
	if (found != std::string::npos)// if there's extension
		ext = path.substr(found);
	if (config.find(ext) != config.end()) //if it's not allowed
		args.push_back(config[ext]);
	args.push_back(path);
	return (args);
}
///////////////////////////////////////////////////////////////////////////

//parses Cgi's response: separates headers from body
//if no valid separation is found, everything is treated as body
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
		return (this->sendError(505), this->_response);
	else
	{
		this->_req = req;
		this->_errorPages = req->getErrorPages();
	}
	if (this->_req->getCode() == 301)//redirect
	{
		this->_response = this->putStatusLine(301);
		this->putGeneralHeaders();
		this->_response += "Location: " + this->_req->getPath() + "\r\n\r\n";
		return (this->_response);
	}
	else if (this->_req->getCode() > 301)
		return (this->sendError(this->_req->getCode()), this->_response);
	std::string	method = this->_req->getMethod();
	if (method == "GET")
		this->_handleGet();
	else if (method == "POST")
		this->_handlePost();
	else if (method == "DELETE")
		this->_handleDelete();
	else
		this->sendError(405);
	std::cout << "\033[33;1mRESPONSE: DONE\033[0m" << std::endl;
	return (this->_response);
}

std::string Response::urlDecode(const std::string &encoded)
{
	std::string	decoded;
	size_t		len = encoded.size();

	for (size_t i = 0; i < len; i++)
	{
		if (encoded[i] == '%')
		{
			decoded += static_cast<char>(strToHex(encoded.substr(i + 1, 2)));
			i += 2;
		}
		else if (encoded[i] == '+')
			decoded += ' ';
		else
			decoded += encoded[i];
	}
	return (decoded);
}


////////////////////// HANDLE REQUESTS BY METHOD ////////////////////////////

//Returns a response with a favicon
void	Response::_handleFavIcon()
{
	std::ifstream	icon("./assets/favicon_general.png");
	std::ostringstream	favicon;

	this->_response = this->putStatusLine(200);
	this->putGeneralHeaders();
	if (!icon.is_open() || this->_isAccepted("image/png") == false)
		this->_body = "";
	else
	{
		favicon << icon.rdbuf();
		this->_body = favicon.str();
		this->_response += "Content-Type: image/png\r\n";
	}
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
	this->_response += this->_body;
}

void	Response::_handleGet()
{
	int	is_dir;
	int code = 0;

	std::cout << "\033[32;1mhandle GET\033[0m" << std::endl;
	if (this->_req->getPath() == "./favicon.ico")//if favicon
		return (void)this->_handleFavIcon();
	std::string	path = this->_req->getPath();
	path = this->urlDecode(path);
	//is_dir = this->_isDir(this->_req->getPath()); //TODO
	is_dir = this->_isDir(path);
	if (is_dir == -1)
		return (void)this->sendError(500);
	else if (is_dir)//if directory
	{
		if (this->_req->getIndex() != "")//if index page
		{
			this->_response = this->putStatusLine(200);
			this->putGeneralHeaders();
			code = this->fileToBody(this->_req->getIndex());
			if (code && code != 404)
				return (void)this->sendError(code);
			if (code == 0)// return index page
			{
				this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
				return (void)(this->_response += this->_body);
			}
		}
		if (this->_req->getAutoIndex() || code == 404)//if autoindex or index page not found
			this->_makeAutoIndex();
		else
			this->sendError(403);
		return ;
	}
	if (this->_req->getCgi() == true) // if there's cgi
	{
		//if (access(this->_req->getPath().c_str(), X_OK))
		//	return (void)this->sendError(403);
		this->_cgiargs = this->_findCgiArgs(this->_req->getPath());
		std::cout << "\033[1;31mRUN CGI, RUUUUUUUN!\033[0m" << std::endl;
		Cgi	cgi(*(this->_req), this->_cgiargs);
		int	cgi_errorPages = cgi.executeCgi(this->_response, TIMEOUT); // execute cgi
		std::cout << "\033[1;31mCGI STATUS " << cgi_errorPages << "\033[0m" << std::endl;
		if (cgi_errorPages) // if cgi returns status != 0 -> error
			return (void)this->sendError(cgi_errorPages);
		this->_parseCgiResponse();
	}
	else //if not cgi
	{
		int error = this->fileToBody(path);
		//int error = this->fileToBody(this->_req->getPath());//TODO
		if (error)
			return (void)this->sendError(error);
	}
	this->putGeneralHeaders();
	if (!this->_body.empty())// if body
	{
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
		this->_response += this->_body;// add body to response
	}
	this->_response.insert(0, this->putStatusLine(200));// put status line
}

void	Response::_handleDelete()
{
	if (access(this->_req->getPath().c_str(), F_OK))// file not found
		return (void)this->sendError(404);
	if (access(this->_req->getPath().c_str(), W_OK))// no permissions
		return (void)this->sendError(403);
	if (std::remove(this->_req->getPath().c_str()))// can't delete file
		return (void)this->sendError(500);
	this->_response = this->putStatusLine(204); // Success + No Content
	this->putGeneralHeaders();
	this->_response += "\r\n\r\n";
}

//>1. GET form
//< return html form
//>2. POST form(?) + body
//< return No Content + html form / No Content + html submission confirmation

// TODO: hi ha una barra de mes
bool	Response::_createFile(void)
{
	std::vector<std::string>	upath = ft_split(this->_req->getUploadDir(), "/");
	std::string	filename = "";
	std::string	create;

	for (std::vector<std::string>::iterator it = upath.begin(); it != upath.end(); it++)
	{
		create = "mkdir ";
		filename += *it + "/";
		if (access(filename.c_str(), F_OK) != 0)//if it doesn't exist
		{
			std::cout << create + filename << std::endl;
			create += filename;
			if (std::system(create.c_str()))
				return (1);
		}
	}
	filename += this->_req->getFileName();
	std::cout << "\033[1;31mFile to create: " << filename << "\033[0m" << std::endl;
	std::ofstream	newfile(filename.c_str());
	if (!newfile.is_open())// creating/opening file failed
			return (1);
	newfile << this->_req->getBody();
	return (0);
}

void	Response::_handlePost()
{
	if (this->_req->getFileName() != "")//if filename
	{
		if (this->_req->getAllowUpload() == false)//no upload permissions
			return (void)this->sendError(403);
		if (this->_createFile())//create file
			return (void)this->sendError(500);
		this->_response = this->putStatusLine(201);
		this->putGeneralHeaders();
		this->putPostHeaders(this->_req->getFileName());
		//ADDED BY JULIA
		this->_response = this->putStatusLine(200);
		this->putGeneralHeaders();
		this->_body = WORK_DONE("File created!");
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
		this->_response += this->_body;
		//FINISH ADDED
	}
	else
	{
		this->_response = this->putStatusLine(200);
		this->putGeneralHeaders();
		this->_body = WORK_DONE("Form submmitted!");
		this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
		this->_response += this->_body;
	}
}

////////////////////////////////////////////////////////////////////////////////

/*
Note: equal_range returns a pair of iterators containing the requested key.
	range.first and range.second are the first and last elements to have
	the requested key, respectively.
Note 2: mime types are divided in type/subtype
First loop:
	Looking for any type (*) paired with either a specific subtype or a *
Second loop:
	Looking for a specific type paired with either a specific subtype or a *
*/
bool	Response::_isAccepted(std::string mime)
{
	std::string::size_type	found = mime.find("/");
	if (found == std::string::npos)//bad format
		return (false);
	std::multimap<std::string, std::string> mp = this->_req->getAcceptedContent();

	std::pair<std::multimap<std::string, std::string>::iterator,
				std::multimap<std::string, std::string>::iterator> range;
	std::multimap<std::string, std::string>::iterator	it;

	range = mp.equal_range("*");// Check for any type
	for (it = range.first; it != range.second; it++)
	{
		if (it->second == "*" || it->second == mime.substr(found + 1))
			return (true);
	}
	range = mp.equal_range(mime.substr(0, found));//Check for specific type
	for (it = range.first; it != range.second; it++)
	{
		if (it->second == "*" || it->second == mime.substr(found + 1))
			return (true);
	}
	return (false);
}

/*Checks if path is a directory. It returns:
-1 if stat returns error
2 if the permissions are limited (shouldn't be displayed)
1 if it's a directory
0 if the file doesn't exist or it doesn't match the previous conditions
*/
int	Response::_isDir(const std::string &path) const
{
	struct stat	status;

	if (access(path.c_str(), F_OK)) //doesn't exist
		return (0);
	if (stat(path.c_str(), &status)) // stat error
		return (-1);
	if (S_ISDIR(status.st_mode)) //it is a directory
		return (1);
	return (0);
}

void	Response::_makeAutoIndex(void)
{
	struct dirent	*dp;
	DIR				*dir;
	int				is_dir;
	std::string		filename;
	std::string		path = this->_req->getPath();

	if (this->_isAccepted("text/html") == false)
		return (void)this->sendError(403);
	if (!(dir = opendir(path.c_str())))
		return (void)this->sendError(500);
	this->_body = AUTOINDEX(path);
	path = ft_strstr(path, this->_req->getRoot());
	if (path.empty())
		path.insert(0, "./");
	else if (path.at(path.size() - 1) != '/')
		path += "/";
	if (path[0] != '.')
		path.insert(0, ".");
	while ((dp = readdir(dir)) != NULL)
	{
		filename = path + dp->d_name;
		if (dp->d_name[0] == '.' || this->_sensitive.count(filename))
			continue ;
		if ((is_dir = this->_isDir(filename)) == -1)
		{
			closedir(dir);
			this->sendError(500);
			return ;
		}
		std::cout << "\033[1;31mAUTO filename: " << filename
			<< " is_dir " << is_dir << " access "
			<< access(filename.c_str(), X_OK) << "\033[0m" << std::endl;
		if (is_dir != 1 && access(filename.c_str(), X_OK) == 0)
			continue ;
		this->_body += AUTOINDEX_FILES(filename.substr(1), dp->d_name);
	}
	closedir(dir);
	this->_body += "</body></html>";
	this->_response = this->putStatusLine(200);
	this->putGeneralHeaders();
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
	this->_response += this->_body;
}

/////////////////////// PUT HEADERS (AND STATUS LINE) //////////////////////////

//puts status line in the response
std::string	Response::putStatusLine(int code)
{
	return ("HTTP/1.1 " + ft_itoa(code) + " " + this->_errorPages[code].first + "\r\n");
}

//puts general header in the response: date, server, keep-alive and connection
void	Response::putGeneralHeaders(void)
{
	std::time_t	date = std::time(NULL);
	this->_response += "Date: ";
	this->_response += std::asctime(std::localtime(&date));
	if (this->_req->getConnectionKeepAlive() == false)
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
	this->_response += "Location: " + this->_req->getUploadDir();
	if (line == "" || mime.eof())
	{
		if (access(file.c_str(), X_OK)) // if not executable
		{
			if (this->_isAccepted("text/plain") == false)
				return (this->sendError(406), 1);
			else
				this->_response += "Content-Type: text/plain\r\n";
		}
		else
		{
			if (this->_isAccepted("application/octet-stream") == false)
				return (this->sendError(406), 1);
			else
				this->_response += "Content-Type: application/octet-stream\r\n";
		}
		return (0);
	}
	line = line.substr(line.find_first_not_of("\t \n\v\r", ext.size()));
	if (this->_isAccepted(line) == false)
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

/*makes error response.
If the error code is not implemented or something happens
generating the error page, a severe internal server error page is sent (505).
If text/html is not accepted, the error will be returned as text/plain or,
if this one is also not accepted, the error will be returned without content.
*/
void	Response::sendError(int code)
{
	int error = 0;
	if (code != 505 && this->_errorPages.find(code) == this->_errorPages.end())
		code = 500;
	this->_code = code;
	if (this->_isAccepted("text/html") == false)
	{
		if (this->_isAccepted("text/plain") == false)
		{
			this->_response = this->putStatusLine(code);
			this->putGeneralHeaders();
			this->_response += "\r\n\r\n";
			return ;
		}
		this->_response = this->putStatusLine(code);
		this->putGeneralHeaders();
		this->_response += "Content-Type: text/plain\r\n";
		this->_response += "Content-Length: " + ft_itoa(this->_errorPages.at(code).first.size()) + "\r\n\n";
		this->_response += ft_itoa(code) + this->_errorPages.at(code).first;
		return ;
	}
	if (code != 505)
		error = fileToBody(this->_errorPages.at(code).second);
	if (code == 505 || (error && fileToBody(this->_errorPages.at(error).second)))//true if we have a double error
	{
		return (void)(this->_response = SEV_ERR, this->_code = 505);
		this->_code = 505;
		return ;
	}
	std::string::size_type	head = this->_body.find("</head>");
	if (head != std::string::npos)
		this->_body.insert(head - 1, "<link rel=\"icon\" type=\"image/png\" href=\"/assets/favicon_error.png\">");
	this->_response = this->putStatusLine(code);
	this->putGeneralHeaders();
	this->_response += "Content-Type: text/html\r\n";
	this->_response += "Content-Length: " + ft_itoa(this->_body.size()) + "\r\n\r\n";
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
