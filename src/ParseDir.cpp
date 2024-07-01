/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseDir.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 15:07:33 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/01 17:41:23 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

// PARSING SIMPLE DIRECTIVES ______________________________________

template <typename T>
void  Parse::errorParse(T &obj, std::vector<std::string> &line)
{
    (void)obj;
    (void)line;
    std::string root = line.back();
    line.pop_back();
    for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); it++)
        obj.setErrorPage(std::atoi((*it).c_str()), root);
    // std::cout << "Error Parse Template ----------------------------" << "\n";
    // std::cout << "Line: " << line << "\n";
}

template <typename T>
void  Parse::rootParse(T &obj, std::vector<std::string> &line)
{
    (void)obj;
    (void)line;
    obj.setRoot(line[1]);
//    std::cout << "Root Parse Template ------------------------------------" << "\n";
//    std::cout << "Line: " << line << "\n";
}

template <typename T>
void  Parse::allowMethodsParse(T &obj, std::vector<std::string> &line)
{
    (void)obj;
    (void)line;
}

template <typename T>
void  Parse::cgiParse(T &obj, std::vector<std::string> &line)
{
    (void)obj;
    (void)line;
}

template <typename T>
void  Parse::autoIndexParse(T &obj, std::vector<std::string> &line)
{
    (void)obj;
    (void)line;
}

void  Parse::allowUploadParse(LocationConfig &serv, std::vector<std::string> &line)
{
    (void)serv;
    (void)line;   
}

void  Parse::hostParse(ServerConfig &serv, std::vector<std::string> &line)
{
    (void)serv;
    (void)line;
    std::string hostname;
    std::string port;

    if (line.size() != 2)
        throw std::invalid_argument("Configuration file error: invalid parameter \"listen\" ");
    size_t  pos = line[1].find(':');
    if (pos != std::string::npos)
    {
        std::vector<std::string> host = ft_split(line[1], ":");
        if (host.size() != 2)
           throw std::invalid_argument("Configuration file error: host not found in \"" + line[1] + "\" ");
        hostname = hostCheck(host[0]);
    }
}
/* if bool - true - we check IP, otherwise only hostname */
std::string     Parse::hostCheck(std::string host)
{
    if (hostNameCheck(host) && ipCheck(host))
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


void  Parse::servNameParse(ServerConfig &serv, std::vector<std::string> &line)
{
    (void)serv;
    (void)line;   
}
void  Parse::bodySizeParse(ServerConfig &serv, std::vector<std::string> &line)
{
    (void)serv;
    (void)line;   
}

void  Parse::uriParse(LocationConfig &loc, std::vector<std::string> &line)
{
    (void)loc;
    (void)line;   
}
void  Parse::indexParse(LocationConfig &loc, std::vector<std::string> &line)
{
    (void)loc;
    (void)line;   
}
void  Parse::returnParse(LocationConfig &loc, std::vector<std::string> &line)
{
    (void)loc;
    (void)line;   
}
void  Parse::uploadDirParse(LocationConfig &loc, std::vector<std::string> &line)
{
    (void)loc;
    (void)line;   
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
