/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:44:49 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/24 21:40:20 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"


std::vector<ServerConfig>	Parse::configParse(char *filename)
{
    std::ifstream				file(filename);
	std::vector<ServerConfig>	sconf;
	std::string					buf;

	if (!file.is_open())
		throw std::invalid_argument("Error when opening config file.");
    buf = checkBrackets(file);
    while (!buf.empty())
	{
 //       std::cout << buf.substr(0, 15) << "\n";
        if (buf.substr(0, 6) != "server")
            throw std::invalid_argument("Configuration file error: no server keyword: " + buf.substr(0, buf.find('\n')));
        // std::stringstream   directive;
        // directive << 
 //       std::cout << "Printing directive: " << directive.str() << "\n" << " ----------------------------------------------" << std::endl;
        ServerConfig	newserv(cropComplexDir(buf));
		if (newserv.empty)
			break ;
		sconf.push_back(newserv);
        std::cout << buf << "\n";
        buf = ltrim(buf);
	}
	return (sconf);
}

std::string Parse::checkBrackets(std::ifstream &filename)
{
	std::string	file;
    int         op = 0;
    int         cl = 0;
	
    if (!std::getline(filename, file, '\0'))
        throw std::invalid_argument("Configuration file error: Empty file");
    for (int i = 0; file[i]; i++)
    {
        if (file[i] == '{')
            op++;
        else if (file[i] == '}')
            cl++;
        if (op - cl < 0)
            break;
    }
    if (op - cl > 0)
        throw std::invalid_argument("Configuration file error: Unclosed bracket");
    if (op - cl < 0)
        throw std::invalid_argument("Configuration file error: Too many closing brackets");
    if (op == 0 && cl == 0)
        throw std::invalid_argument("Configuration file error: no brackets found");
    file = trim(file);
    return (file);
}



// Function to crop the config in the brackets (location or server)
// Returns the cropped part without brackets and trims this part from the buf
std::string   Parse::cropComplexDir(std::string &buf)
{
    size_t pos = buf.find('{');
//    std::cout << pos << "\n";
    if (pos == std::string::npos)
        throw std::invalid_argument("Configuration file error: missing bracket after a keyword: " + buf.substr(0, buf.find('\n')));
 //   std::cout << buf.substr(0, 15) << "\n";
    buf = buf.substr(pos + 1);
    pos = 0;
    int br = 1;
 //   std::cout << buf.substr(0, 15) << "\n";
    while (br > 0 && buf[pos])
    {
        if (buf[pos] == '{')
            br++;
        if (buf[pos] == '}')
            br--;
        pos++;
    }
 //   std::cout << pos << "\n";
    if (br)
        throw std::invalid_argument("Configuration file error: missing brackets");
    
    std::string   complexDir = buf.substr(0, pos - 1);
    buf = buf.substr(pos);
    return (complexDir);
}

// template <typename T>
//     static void  errorParse(T &obj, std::string &line);

//     template <typename T>
//     static void  rootParse(T &obj, std::string &line);

//     template <typename T>
//     static void  allowMethodsParse(T &obj, std::string &line);

//     template <typename T>
//     static void  cgiParse(T &obj, std::string &line);

//     template <typename T>
//     static void  boolParse(T &obj, std::string &line, int flag); // switch: 0 - autoindex, 1 - upload

//     static void  hostParse(ServerConfig &serv, std::string &line);
//     static void  servNameParse(ServerConfig &serv, std::string &line);
//     static void  bodySizeParse(ServerConfig &serv, std::string &line);
//     static void  locationParse(ServerConfig &serv, std::string &line);

//     static void  uriParse(LocationConfig &serv, std::string &line);
//     static void  indexParse(LocationConfig &serv, std::string &line);
//     static void  aliasParse(LocationConfig &serv, std::string &line);
