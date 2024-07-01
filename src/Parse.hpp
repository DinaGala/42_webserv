/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/01 17:42:10 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include "Utils.hpp"

# define INDEX 0
# define UPLOAD 1
# define WS " \v\t\n\r"

class ServerConfig;
class LocationConfig;

// before setting unique vars check if they are empty or repeated
class Parse
{
    public: 

    /* MAIN PARSING ENGINE: PARSING COMPLEX DIRECTIVES ______________________*/
        static std::vector<ServerConfig>	configParse(char *name);
        template <typename T>
        static void complexParse(T &serv, std::string &block);       
        template <typename T>
        static void blockParse(T &serv, std::string &line);
        template <typename T>
        static void lineParse(T &obj, std::vector<std::string> args);

    /* PARSING UTILS ________________________________________________________*/
        static int          ft_getline(std::string &buf, std::string &line, std::string del); //returns 0 - empty line, 1 - first delimiter, 2  - second delimiter
        static std::string  ft_getword(std::string &buf);
        static std::string  checkBrackets(std::ifstream &filename);
        static std::string  blockCrop(std::string &buf);
        static std::vector<std::string>  checkComment(std::vector<std::string> line); 

    /* PARSING SIMPLE DIRECTIVES - ParseDir.cpp _______________________________*/
        template <typename T>
        static void  errorParse(T &obj, std::vector<std::string> &line);
        template <typename T>
        static void  rootParse(T &obj, std::vector<std::string> &line);
        template <typename T>
        static void  allowMethodsParse(T &obj, std::vector<std::string> &line);
        template <typename T>
        static void  cgiParse(T &obj, std::vector<std::string> &line);
        template <typename T>
        static void  autoIndexParse(T &obj, std::vector<std::string> &line);

        static void  hostParse(ServerConfig &serv, std::vector<std::string> &line);
        static std::string  hostCheck(std::string host);
        static bool   hostNameCheck(std::string host);
        static bool   ipCheck(std::string host);
        static void  servNameParse(ServerConfig &serv, std::vector<std::string> &line);
        static void  bodySizeParse(ServerConfig &serv, std::vector<std::string> &line);

        static void  uriParse(LocationConfig &loc, std::vector<std::string> &line);
        static void  indexParse(LocationConfig &loc, std::vector<std::string> &line);
        static void  returnParse(LocationConfig &loc, std::vector<std::string> &line);
        static void  uploadDirParse(LocationConfig &loc, std::vector<std::string> &line);
        static void  allowUploadParse(LocationConfig &serv, std::vector<std::string> &line);

};

/* OVERLOADED OPERATORS _____________________________________________________*/
std::ostream	&operator<<(std::ostream &out, const std::vector<std::string> &val);
std::ostream	&operator<<(std::ostream &out, std::map<std::string, std::string> &val);
std::ostream	&operator<<(std::ostream &out, std::map<int, std::string> &val);
std::ostream	&operator<<(std::ostream &out, const ServerConfig &val);
std::ostream	&operator<<(std::ostream &out, const LocationConfig &val);
std::ostream	&operator<<(std::ostream &out, const std::vector<ServerConfig> &val);

#endif
