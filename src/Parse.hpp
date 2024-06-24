/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/24 22:19:08 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include "Utils.hpp"

# define INDEX 0
# define UPLOAD 1

class ServerConfig;
class LocationConfig;

typedef void (*funcs)();

// before setting unique vars check if they are empty or repeated
class Parse
{
    template <typename T>
    static void  errorParse(T &obj, std::string &line);

    template <typename T>
    static void  rootParse(T &obj, std::string &line);

    template <typename T>
    static void  allowMethodsParse(T &obj, std::string &line);

    template <typename T>
    static void  cgiParse(T &obj, std::string &line);

    template <typename T>
    static void  boolParse(T &obj, std::string &line, int flag); // switch: 0 - autoindex, 1 - upload

    static void  hostParse(ServerConfig &serv, std::string &line);
    static void  servNameParse(ServerConfig &serv, std::string &line);
    static void  bodySizeParse(ServerConfig &serv, std::string &line);
    static void  locationParse(ServerConfig &serv, std::string &line);

    static void  uriParse(LocationConfig &serv, std::string &line);
    static void  indexParse(LocationConfig &serv, std::string &line);
    static void  aliasParse(LocationConfig &serv, std::string &line);

    static std::vector<ServerConfig>	configParse(char *name);
    static std::string cropComplexDir(std::string &buf);
    static std::string ft_getline(std::string &buf, int flag); // flag 0 - both {} and ;, 1 - only ;
    static std::string checkBrackets(std::ifstream &filename);
};
// void        exceptionFormat()

#endif
