/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:44:49 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/26 21:44:09 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

// MAIN PARSING ENGINE: PARSING COMPLEX DIRECTIVES ______________________________________

/* Main function: splits the config file in server blocks and saves it in a vector 
*/
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
		sconf.push_back( ServerConfig(blockCrop(buf)));
//        std::cout << buf << "\n";
        buf = ltrim(buf);
	}
	return (sconf);
}

template <typename T>
void Parse::complexParse(T &serv, std::string &block)
{
    std::string line;
    switch (ft_getline(block, line, ";{"))
    {
        case 1: lineParse(serv, checkComment(ft_split(line, " \v\t\n\r"))); break ;
        case 2: blockParse(serv, line); break ;
        default: std::cout << "The server block is parsed" << std::endl; return ;
    }
   complexParse(serv, block);
}

template <typename T>
void Parse::lineParse(T &obj, std::vector<std::string> args)
{
    if (args.empty())
        return ;
    // std::cout << "Line Parse -----------------" << "\n";
    // std::cout << "Line: " << args << "\n";

    typename std::map<std::string, typename T::func>::iterator it;
    std::map<std::string, typename T::func> keys = obj.getKeys();
    for (it = keys.begin(); it != keys.end(); it++)
    {
        if (it->first == args[0])
        {
            it->second(obj, args); // change everywhere strings to vectors
            return ;
        }
    }
    throw std::invalid_argument("Configuration file error: no such keyword recognized: " + args[0]);
}

template <typename T>
void Parse::blockParse(T &serv, std::string &line)
{
    if (line.substr(0, 8) != "location")
        throw std::invalid_argument("Configuration file error: no location keyword: " + line.substr(0, line.find('\n')));
    if (!serv.loc)
        throw std::invalid_argument("Configuration file error: location inside of a location not permitted: " + line.substr(0, line.find('\n')));
    line.erase(0, 8);
    if (ft_getword(line).empty())
        throw std::invalid_argument("Configuration file error: no location url" + line.substr(0, line.find('\n')));
    serv.setLocationConfig(LocationConfig(ft_getword(line), blockCrop(line)));
}

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
void  Parse::autoIndexParse(T &obj, std::vector<std::string> &line) // switch: 0 - autoindex, 1 - upload
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


// PARSING UTILS ______________________________________

int Parse::ft_getline(std::string &buf, std::string &line, std::string del)
{
    buf = trim(buf);
    if (buf.empty())
        return (0);
    // std::cout << "FT GET LINE START ___________________" << "\n";
    // std::cout << "block: " << buf << "\n";
    size_t  pos = buf.find_first_of(del);
    int res = 1;
    if (pos == std::string::npos)
    {
        line = buf.substr(0, buf.length());
        buf.clear();
    }
    else 
    {
        if (buf[pos] == '{')
        {
            pos = buf.find('}') + 1;
            // std::cout << "BUF pos " << buf[pos - 1] << "\n";
            res = 2;
        }
        line = buf.substr(0, pos);
        buf.erase(0, line.length() + 1);
    }
    // std::cout << "FT GET LINE END" << "\n";
    // std::cout << "Line: " << line << "\n";
    return (res);
}

std::string Parse::ft_getword(std::string &buf)
{
    std::string str = trim(buf);
    size_t pos = str.find_first_of(" \v\t\n\r");
    if (pos != std::string::npos)
        str = str.substr(0, pos);
    return (str);
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
std::string   Parse::blockCrop(std::string &buf)
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

std::vector<std::string>  Parse::checkComment(std::vector<std::string> line)
{
    for (std::vector<std::string>::iterator it = line.begin(); it != line.end();)
    {
        if (it->empty() || (*it)[0] == '#')
            it = line.erase(it);
        else
            it++;
    }
    return (line);
}


// OVERLOADED OPERATORS ______________________________________

std::ostream	&operator<<(std::ostream &out, const std::vector<std::string> &val)
{
    for (unsigned int i = 0; i < val.size(); i++)
		out << val[i] << ", ";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::vector<int> &val)
{
    for (unsigned int i = 0; i < val.size(); i++)
		out << val[i] << ", ";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::map<std::string, std::string> &valinit)
{
    std::map<std::string, std::string> val = valinit;
    for (std::map<std::string, std::string>::iterator it = val.begin(); it != val.end(); it++)
		out << "   [\"" + it->first + "\"] = " + it->second << "\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::map<int, std::string> &valinit)
{
    std::map<int, std::string> val = valinit;
    for (std::map<int, std::string>::iterator it = val.begin(); it != val.end(); it++)
		out << "   [\"" << it->first << "\"] = " + it->second << "\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const ServerConfig &val)
{
    out << "Port:  " << val.getPort() << "\n";
    out << "Host:  " << val.getHost() << "\n";
    out << "Root:  " << val.getRoot() << "\n";
    out << "Max Body Size:  " << val.getMaxBodySize() << "\n";
    out << "Allowed methods:  " << val.getAllowedMethods() << "\n";
    out << "Error pages:  \n" << val.getErrorPages() << "\n";
    out << "CGI:  " << val.getCgiConf() << "\n";
    std::vector<LocationConfig> temp = val.getLocationConfig();
    for (std::vector<LocationConfig>::iterator it = temp.begin(); it != temp.end(); it++) {
        out << "LOCATION \n" << (*it) << "\n";
    }
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::vector<ServerConfig> &val)
{
    std::vector<ServerConfig> temp = val;
    for (std::vector<ServerConfig>::iterator it = temp.begin(); it != temp.end(); it++) {
        out << "SERVER ----------------------\n" << (*it) << "\n";
    }
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const LocationConfig &val)
{
    out << "    |___    URL:  " << val.getUri() << "\n";
    out << "    |___    Root:  " << val.getRoot() << "\n";
    out << "    |___    Return:  " << val.getReturn() << "\n";
    out << "    |___    Index:  " << val.getIndex() << "\n";
    out << "    |___    Upload dir:  " << val.getUploadDir() << "\n";
    out << "    |___    Allow upload:  " << val.getAllowUpload() << "\n";
    out << "    |___    Autoindex:  " << val.getAutoIndex() << "\n";
    out << "    |___    Allowed methods:  " << val.getAllowedMethods() << "\n";
    out << "    |___    Error pages:  \n         " << val.getErrorPages() << "\n";
    out << "    |___    CGI:  " << val.getCgiConf() << "\n";

	return (out);
}

// TEMPLATE PROTOTYPES ______________________________________

template void Parse::complexParse<ServerConfig>(ServerConfig &serv, std::string &block);
template void Parse::complexParse<LocationConfig>(LocationConfig &serv, std::string &block);
template void Parse::lineParse<ServerConfig>(ServerConfig &serv, std::vector<std::string> args);
template void Parse::lineParse<LocationConfig>(LocationConfig &serv, std::vector<std::string> args);
template void Parse::blockParse<ServerConfig>(ServerConfig &serv, std::string &line);
template void Parse::blockParse<LocationConfig>(LocationConfig &serv, std::string &line);
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
