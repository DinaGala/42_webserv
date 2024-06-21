/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/21 21:04:05 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

# include "Utils.hpp"

# define INDEX 0
# define UPLOAD 1

class ServerConfig;
class LocationConfig;

typedef void (*funcs)();

// before setting unique vars check if they are empty or repeated

template <typename T>
void  errorParse(T &obj, std::string &line);

template <typename T>
void  rootParse(T &obj, std::string &line);

template <typename T>
void  allowMethodsParse(T &obj, std::string &line);

template <typename T>
void  cgiParse(T &obj, std::string &line);

template <typename T>
void  boolParse(T &obj, std::string &line, int flag); // switch: 0 - autoindex, 1 - upload

void  hostParse(ServerConfig &serv, std::string &line);
void  servNameParse(ServerConfig &serv, std::string &line);
void  bodySizeParse(ServerConfig &serv, std::string &line);
void  locationParse(ServerConfig &serv, std::string &line);

void  uriParse(LocationConfig &serv, std::string &line);
void  indexParse(LocationConfig &serv, std::string &line);
void  aliasParse(LocationConfig &serv, std::string &line);

std::vector<ServerConfig>	configParse(char *name);
std::string cropComplexDir(std::string &buf);
std::string ft_getline(std::string &buf, int flag); // flag 0 - both {} and ;, 1 - only ;
std::string checkBrackets(std::ifstream &filename);

// void        exceptionFormat()

#endif