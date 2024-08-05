#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#ifndef MAXCONNECT
# define MAXCONNECT 10
#endif

# define AUTOINDEX(content) std::string("<html><head><title>AUTOINDEX</title><h1 style=\"text-align:center;font-size:200%;\">Index of ") + content + std::string("</h1></head><body style=\"font-size:150%;margin:50px;\">")
# define AUTOINDEX_FILES(href, name) std::string("<p><a href= ") + href + std::string(">") + name + std::string("</a></p>\n")

#include <map>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include "Utils.hpp"


class Request;

class	Response
{
	public:
		Response();
		Response(const Response &r);
		//Response(Request &req);
		~Response();

		Response	&operator=(const Response &r);
		//SET VARS
		void		setBody(const std::string &msg);
		void		setCgiPath(const std::string &cgi);
		void		setCode(const int &code);
		void		setReq(const Request *rqt);
		const std::string& 		getResponse() const;
		int				getCode() const;
		//WRITE RESPONSE
		std::string	putStatusLine(int code);
		void		putGeneralHeaders(void);
		bool		putPostHeaders(const std::string &file);
		int			fileToBody(const std::string &path);
		//SEND RESPONSE
		void		cleanResponse(); // TODO for multiplexing

		std::string	&makeResponse(const Request *req);
		void		sendError(int code);
		std::string	ft_strnstr(const std::string &h, const std::string &n);

	private:
		//static std::map<int, std::pair<std::string, std::string> >	_status;
		//static std::map<int, std::pair<std::string, std::string> >	_initStatus();
		std::map<int, std::pair<std::string, std::string> >	_status;
		std::string		_body;
		std::string		_reqbody;
		std::string		_response;
		std::vector<std::string>	_cgiargs;
		unsigned int	_code;
		const Request	*_req;

		void		_parseCgiResponse(void);
		void		_handleGet(void);
		void		_handlePost(void);
		void		_handleDelete(void);
		std::string	_parseUrl(const std::string &url);
		std::vector<std::string> _findCgiArgs(const std::string &path);
		bool		_createFile(void);
		bool		_isAccepted(std::string str);
		void		_makeAutoIndex(void);
		int			_isDir(const std::string &path) const ;
		void		_handleFavIcon();
};

#endif
