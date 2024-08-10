#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#ifndef MAXCONNECT
# define MAXCONNECT 10
#endif

# define AUTOINDEX(content) \
	std::string("<html><head><title>AUTOINDEX</title><h1 style=\"text-align:center;font-size:200%;\">Index of ") \
			+ content + std::string("</h1></head><body style=\"font-size:150%;margin:50px;\">")
# define AUTOINDEX_FILES(href, name) \
	std::string("<p><a href= \'") + href + std::string("\'>") + name + std::string("</a></p>\n")
# define SEV_ERR "HTTP/1.1 505 Severe Internal Server Error\r\n" \
				"Content-Type: text/plain\r\nContent-Length: 47\r\n\r\n" \
				"Severe Internal Error.\nPlease, try again later\n"

#include <map>
#include <set>
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
		Response	&operator=(const Response &r);
		~Response();

		std::string	&makeResponse(const Request *req);
		void		cleanResponse(); // TODO for multiplexing
		/************** GETTERS *******************/
		const std::string& 		getResponse() const;
		int						getCode() const;

	private:
		std::map<int, std::pair<std::string, std::string> >	_status;
		static const std::set<std::string>	_sensitive;
		static const std::set<std::string>	_initSensitive();
		std::vector<std::string>	_cgiargs;
		std::string		_body;
		std::string		_response;
		unsigned int	_code;
		const Request	*_req;

		/************ WRITE RESPONSE **************/
		std::string	putStatusLine(int code);
		void		putGeneralHeaders(void);
		bool		putPostHeaders(const std::string &file);
		int			fileToBody(const std::string &path);
		void		sendError(int code);
		/************ HANDLE METHODS **************/
		void		_handleGet(void);
		void		_handlePost(void);
		void		_handleDelete(void);
		/***************** CGI *******************/
		std::vector<std::string> _findCgiArgs(const std::string &path);
		void		_parseCgiResponse(void);
		/*********** METHODS' UTILS **************/
		bool		_createFile(void);
		bool		_isAccepted(std::string str);
		int			_isDir(const std::string &path) const ;
		std::string	urlDecode(const std::string &path);
		/************ SPECIAL REQUESTS **********/
		void		_makeAutoIndex(void);
		void		_handleFavIcon();
};

#endif
