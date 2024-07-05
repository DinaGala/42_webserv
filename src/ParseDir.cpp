/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseDir.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:07:33 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/05 14:39:02 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

// PARSING SIMPLE DIRECTIVES ______________________________________

template <typename T>
void  Parse::errorParse(T &obj, std::vector<std::string> &line)
{
	if (line.size() < 3)
		throw std::invalid_argument("Configuration file error: invalid parameter \"error_page\": enter at least 2 parameters: [error_num] ... [path]");
	std::string root = line.back();
	line.pop_back();
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); it++)
	{
		if (it == line.begin())
			continue ;
		obj.setErrorPage(errorCheck(*it), root);
	}
		
	// std::cout << "Error Parse Template ----------------------------" << "\n";
	// std::cout << "Line: " << line << "\n";
}

int		Parse::errorCheck(std::string er)
{
	try
	{
//		std::cout << er << "\n";
		int res = ft_atopi(er);
		if (res < 300 || res > 599)
			throw std::invalid_argument("");
		return (res);
	}
	catch(const std::exception& e)
	{
		throw std::invalid_argument("Configuration file error: error value must be between 300 and 599");
	}
}

template <typename T>
void  Parse::rootParse(T &obj, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"root\": enter only one parameter");
	obj.setRoot(line[1]);
//    std::cout << "Root Parse Template ------------------------------------" << "\n";
//    std::cout << "Line: " << line << "\n";
}

template <typename T>
void  Parse::allowMethodsParse(T &obj, std::vector<std::string> &line)
{
	if (line.size() < 2)
		throw std::invalid_argument("Configuration file error: invalid number of parameters \"allow_methods\": enter at least one parameter");
	std::map<std::string, bool> temp = obj.getVars();
//	std::cout << "ALLOW METHODS, LINE: " << line << std::endl;
	if (temp["allow_methods"])
		throw std::invalid_argument("Configuratiosn file error: \"allow_methods\" directive is duplicate ");
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); it++)
	{
		if (it == line.begin())
			continue ;
		obj.setAllowMethod(*it);
	}
	obj.setVars("allow_methods");
}

template <typename T>
void  Parse::cgiParse(T &obj, std::vector<std::string> &line)
{
	if (line.size() != 3)
		throw std::invalid_argument("Configuration file error: invalid parameter \"cgi\": enter two parameters [.ext] [path]");
	obj.setCgiConf(line[1], line[2]);
}

template <typename T>
void  Parse::autoIndexParse(T &obj, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"autoindex\" ");
	if (line[1] == "true")
		obj.setAutoIndex(true);
	else if (line[1] == "false")
		obj.setAutoIndex(false);
	else
		throw std::invalid_argument("Configuration file error: invalid parameter \"autoindex\": " + line[1]);
}

void  Parse::allowUploadParse(LocationConfig &serv, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"allow_upload\" ");
	if (line[1] == "true")
		serv.setAllowUpload(true);
	else if (line[1] == "false")
		serv.setAllowUpload(false);
	else
		throw std::invalid_argument("Configuration file error: invalid parameter \"allow_upload\": " + line[1]);
}

void  Parse::hostParse(ServerConfig &serv, std::vector<std::string> &line)
{
	int port;

	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"listen\" ");
	size_t  pos = line[1].find(':');
	if (pos != std::string::npos)
	{
		std::vector<std::string> host = ft_split(line[1], ":");
		if (host.size() != 2)
		   throw std::invalid_argument("Configuration file error: host not found in \"" + line[1] + "\" ");
		serv.setHost(host[0]);
		port = portCheck(host[1]);
	}
	else
		port = portCheck(line[1]);
	serv.setPort(port);
}

/*  */
std::string     Parse::hostCheck(std::string host)
{
	if (!hostNameCheck(host) && !ipCheck(host))
		throw std::invalid_argument("Configuration file error: host not found in " + host);
	return (host);
}

bool    Parse::hostNameCheck(std::string host)
{
	for (int i = 0; host[i]; i++)
	{
		if (std::isdigit(host[i]) || (host[i] > 64 && host[i] < 91) \
		|| (host[i] > 96 && host[i] < 123) || host[i] == '.' || host[i] == '-')
			continue ;
		else
			return (false);
	}
	return (true);
}

bool    Parse::ipCheck(std::string host)
{
	int dot = 0;
	int num = 0;

	for (int i = 0; host[i]; i++)
	{
		if (host[i] == '.' && num != 0)
		{
			dot++;
			num = 0;
		}
		else if (std::isdigit(host[i]))
			num = num * 10 + (host[i] - 48);
		else
			return (false);
		if (num > 255)
			return (false);
	}
	return (true);
}

int     Parse::portCheck(std::string port)
{
	int res;
	
	try
	{
		res = ft_atopi(port);
		if (res < 0 || res > 65535)
			throw std::invalid_argument("");  
		return (res);
	}
	catch(const std::exception& e)
	{
		throw std::invalid_argument("Configuration file error: invalid port \"" + port + "\" in \"listen\" directive");
	}
}

void  Parse::servNameParse(ServerConfig &serv, std::vector<std::string> &line)
{
	if (line.size() < 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"server_name\": enter at least one parameter ");
	std::vector<std::string>::iterator	it = line.begin();
	it++;
	while (it != line.end())
	{
		serv.setServerName(*it);
		it++;
	}
		
}

void  Parse::bodySizeParse(ServerConfig &serv, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"client_max_body_size\": enter only one parameter ");
	size_t	pos = line[1].find_first_not_of(NUM);
	int		format = 1;
	long long	sz;
	std::string num = line[1];

	if (pos != std::string::npos)
	{
		num = line[1].substr(0, pos);
		if (!line[1][pos + 1] && (line[1][pos] == 'k' || line[1][pos] == 'K'))
			format = 1000;
		else if (!line[1][pos + 1] && (line[1][pos] == 'm' || line[1][pos] == 'M'))
			format = 1000000;
		else if (!line[1][pos + 1] && (line[1][pos] == 'g' || line[1][pos] == 'G'))
			format = 1000000000;
		else
			throw std::invalid_argument("Configuration file error: invalid parameter \"client_max_body_size\": the value can be followed only by k | K | m | M | g | G" + line[1]);
	}
	try
	{
		sz = ft_atopi(num);
		if (sz * format > MAX_BODY_SIZE)
			throw std::invalid_argument("");
		serv.setClientMaxBodySize(sz * format);
	}
	catch(const std::exception& e)
	{
		throw std::invalid_argument("Configuration file error: invalid parameter \"client_max_body_size\": the value  is out of the limits (up to 2000000000 bytes): " + line[1]);
	}
}

// void  Parse::uriParse(LocationConfig &loc, std::vector<std::string> &line)
// {
// 	(void)loc;
// 	(void)line;   
// }
void  Parse::indexParse(LocationConfig &loc, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"index\": introduce one parameter");
	loc.setIndex(line[1]);
}
void  Parse::returnParse(LocationConfig &loc, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"return\": introduce one parameter");
	loc.setReturn(line[1]);
}
void  Parse::uploadDirParse(LocationConfig &loc, std::vector<std::string> &line)
{
	if (line.size() != 2)
		throw std::invalid_argument("Configuration file error: invalid parameter \"upload_dir\": introduce one parameter");
	loc.setUploadDir(line[1]);
}

std::string	Parse::isHostName()
{
	return (isLetDig() + ".-");
}
std::string	Parse::isPath()
{
	return (isHostName() + "/_");
}
std::string	Parse::isUrl()
{
	return (isPath() + ":?=&");
}

// TEMPLATE PROTOTYPES ______________________________________

template void Parse::rootParse<ServerConfig>(ServerConfig &serv, std::vector<std::string>&line);
template void Parse::rootParse<LocationConfig>(LocationConfig &serv, std::vector<std::string>&line);
template void Parse::errorParse<ServerConfig>(ServerConfig &serv, std::vector<std::string>&line);
template void Parse::errorParse<LocationConfig>(LocationConfig &serv, std::vector<std::string>&line);
template void Parse::autoIndexParse<ServerConfig>(ServerConfig &serv, std::vector<std::string>&line);
template void Parse::autoIndexParse<LocationConfig>(LocationConfig &serv, std::vector<std::string>&line);
template void Parse::allowMethodsParse<ServerConfig>(ServerConfig &serv, std::vector<std::string>&line);
template void Parse::allowMethodsParse<LocationConfig>(LocationConfig &serv, std::vector<std::string>&line);
template void Parse::cgiParse<ServerConfig>(ServerConfig &serv, std::vector<std::string>&line);
template void Parse::cgiParse<LocationConfig>(LocationConfig &serv, std::vector<std::string>&line);
