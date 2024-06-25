/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/25 21:49:13 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include "Utils.hpp"

# define INDEX 0
# define UPLOAD 1

class ServerConfig;
class LocationConfig;

typedef void (*funcS)(ServerConfig &, std::string &);
typedef void (*funcL)(LocationConfig &, std::string &);

// before setting unique vars check if they are empty or repeated
class Parse
{
    public: 

        template <typename T>
        static void  errorParse(T &obj, std::string &line);
        static void  errorParse(ServerConfig &serv, std::string &line);
        static void  errorParse(LocationConfig &loc, std::string &line);

        template <typename T>
        static void  rootParse(T &obj, std::string &line);
        static void  rootParse(ServerConfig &serv, std::string &line);
        static void  rootParse(LocationConfig &loc, std::string &line);

        template <typename T>
        static void  allowMethodsParse(T &obj, std::string &line);
        static void  allowMethodsParse(LocationConfig &serv, std::string &line);
        static void  allowMethodsParse(ServerConfig &serv, std::string &line);
        

        template <typename T>
        static void  cgiParse(T &obj, std::string &line);
        static void  cgiParse(ServerConfig &serv, std::string &line);
        static void  cgiParse(LocationConfig &serv, std::string &line);

        template <typename T>
        static void  boolParse(T &obj, std::string &line, int flag); // switch: 0 - autoindex, 1 - upload
        static void  autoIndexParse(ServerConfig &serv, std::string &line);
        static void  autoIndexParse(LocationConfig &serv, std::string &line);
        static void  allowUploadParse(LocationConfig &serv, std::string &line);

        static void  hostParse(ServerConfig &serv, std::string &line);
        static void  servNameParse(ServerConfig &serv, std::string &line);
        static void  bodySizeParse(ServerConfig &serv, std::string &line);
 //       static void  locationParse(ServerConfig &serv, std::string &line);

        static void  uriParse(LocationConfig &loc, std::string &line);
        static void  indexParse(LocationConfig &loc, std::string &line);
        static void  returnParse(LocationConfig &loc, std::string &line);
        static void  uploadDirParse(LocationConfig &loc, std::string &line);

        static std::vector<ServerConfig>	configParse(char *name);
        static std::string blockCrop(std::string &buf);
        static int ft_getline(std::string &buf, std::string &line, std::string del); //returns 0 - empty line, 1 - first delimiter, 2  - second delimiter
        static std::string ft_getword(std::string &buf);
        static std::string checkBrackets(std::ifstream &filename);
        
        template <typename T>
        static void complexParse(T &serv, std::string &block);
        
        template <typename T>
        static void blockParse(T &serv, std::string &line);

        template <typename T>
        static void lineParse(T &obj, std::vector<std::string> &args);
};
// void        exceptionFormat()

#endif
