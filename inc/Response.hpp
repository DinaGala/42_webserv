#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#ifndef MAXCONNECT
# define MAXCONNECT 10
#endif

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
#include "RespUtils.hpp"


class Request;

class	Response
{
	public:
		Response();
		Response(const Response &r);
		Response	&operator=(const Response &r);
		~Response();

		std::string	&makeResponse(const Request *req);
		void		cleanResponse();
		/************** GETTERS *******************/
		const std::string& 		getResponse() const;
		int						getCode() const;

	private:
		std::map<int, std::pair<std::string, std::string> >	_errorPages;
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
