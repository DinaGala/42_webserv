/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:44:49 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/17 15:42:55 by nuferron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Parse.hpp"

// MAIN PARSING ENGINE: PARSING COMPLEX DIRECTIVES ______________________________________

/* Main function: splits the config file in server blocks and saves it in a vector 
*/
std::vector<ServerConfig>	Parse::configParse(const char *filename)
{
    std::ifstream				file(filename);
	std::vector<ServerConfig>	sconf;
	std::string					buf;

	if (!file.is_open())
		throw std::invalid_argument("Error when opening config file.");
    buf = checkBrackets(file);
    while (!buf.empty())
	{
        if (buf.substr(0, 6) != "server")
            throw std::invalid_argument("Configuration file error: no server keyword: " + buf.substr(0, buf.find('\n')));
		sconf.push_back( ServerConfig(blockCrop(buf)));
        buf = ltrim(buf);
	}
    sconf = portDefault(sconf); 
    sconf = hostDefault(sconf); 
	return (sconf);
}

template <typename T>
void Parse::complexParse(T &serv, std::string &block)
{
    std::string line;
    switch (ft_getline(block, line, ";{"))
    {
        case 1: lineParse(serv, ft_split(line, WS)); break ;
        case 2: blockParse(serv, line); break ;
        default: return ;
    }
   complexParse(serv, block);
}

template <typename T>
void Parse::lineParse(T &obj, std::vector<std::string> args)
{
    if (args.empty())
        return ;

    typename std::map<std::string, typename T::func>::iterator it;
    std::map<std::string, typename T::func> keys = obj.getKeys();
    for (it = keys.begin(); it != keys.end(); it++)
    {
        if (it->first == args[0])
        {
            it->second(obj, args);
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
    std::string path = ft_getword(line, " \v\t\n\r{");
    if (path.empty())
        throw std::invalid_argument("Configuration file error: no location url" + line.substr(0, line.find('\n')));
    serv.setLocationConfig(LocationConfig(path, serv.getErrorPages(), blockCrop(line)));
}

std::vector<ServerConfig>	Parse::portDefault(std::vector<ServerConfig> &sconf)
{
    std::vector<int> p;

    for (std::vector<ServerConfig>::iterator it = sconf.begin(); it != sconf.end();)
	{
		std::map<std::string, bool> svar = it->getVars();
        if (svar["port"] == false)
			it->setPort(8080);
        if (checkDupsPort(*it, p))
            it = sconf.erase(it);
        else
            it++;
	}

    return (sconf);
}

std::vector<ServerConfig>	Parse::hostDefault(std::vector<ServerConfig> &sconf)
{
    for (std::vector<ServerConfig>::iterator it = sconf.begin(); it != sconf.end(); it++)
	{
		std::map<std::string, bool> svar = it->getVars();
        if (!svar["host"] && !svar["ip"])
        {
            it->setHost("localhost");
            it->setIp(findIp("localhost", true));
        }
        else if (svar["host"] && !svar["ip"])
            it->setIp(findIp(it->getHost(), true));
        else if (!svar["host"] && svar["ip"])
            it->setHostName(findIp(it->getIp(), false));
	}

    return (sconf);
}

// if flag = true - we are looking for ip, 
// if flag = false - we are looking for the hostname
std::string	Parse::findIp(std::string host, bool flag)
{
    std::ifstream file("/etc/hosts");
    if (!file.is_open())
    {
        file.open(".hosts");
        std::cout << "Couldn't find /etc/hosts, used local host file.\n";
    }
    if (!file.is_open())
        throw std::invalid_argument("Error: couldn't open the hosts file to find the host \"" + host + "\"");
    std::string line;
    while(getline(file, line))
    {
        if (line.empty() || trim(line).empty() || trim(line)[0] == '#')
            continue ;
        std::vector<std::string>    arr = ft_split(trim(line), WS);
        if (arr.size() < 2)
            continue ;
        if (arr[1] == host && flag == true)
        {
            file.close();
            return (arr[0]);
        }
        if (arr[0] == host && flag == false)
        {
            file.close();
            return (arr[1]);
        }
    }
    file.close();
    throw std::invalid_argument("Configuration file error: host not found in \"" + host + "\" of the \"listen\" directive");
    return (line);
}


int  Parse::checkDupsPort(ServerConfig &serv, std::vector<int> &all)
{
    std::vector<int> ports = serv.getPort();
    for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
    {
        if (std::find(all.begin(), all.end(), *it) != all.end()) 
        {
            std::cout << "Attention: port " << *it << " is already used by another server, ignored\n";
            serv.unsetPort(*it);
        }   
        else 
        {
            all.push_back(*it);
        }
    }
    if (serv.getPort().empty() && std::find(all.begin(), all.end(), 8080) != all.end())
        return (1);
    else if (serv.getPort().empty())
        serv.setPort(8080);
    return (0);
}

// PARSING UTILS ______________________________________

int Parse::ft_getline(std::string &buf, std::string &line, std::string del)
{
    buf = trim(buf);
    if (buf.empty())
        return (0);
    size_t  pos = buf.find_first_of(del);
    int res = 1;
    if (pos == std::string::npos)
        throw std::invalid_argument("Configuration file error: unfinished line " + line.substr(0, line.find('\n')) + ". Use a delimiter: \"" + del + "\"");
    else if (pos == 0)
        throw std::invalid_argument("Configuration file error: unexpected \";\"");
    else
    {
        if (buf[pos] == '{')
        {
            pos = buf.find('}') + 1;
            res = 2;
        }
        line = buf.substr(0, pos);
        buf.erase(0, line.length() + 1);
    }
    return (res);
}

std::string Parse::ft_getword(std::string &buf, std::string del)
{
    std::string str = trim(buf);
    size_t pos = str.find_first_of(del);
    if (pos != std::string::npos)
        str = str.substr(0, pos);
    return (str);
}

/* Aux function that trims the comments and checks the brackets */

std::string Parse::checkBrackets(std::ifstream &filename)
{
	std::string	file;
    std::string line;
    int         op = 0;
    int         cl = 0;
	
    if (!std::getline(filename, line))
        throw std::invalid_argument("Configuration file error: Empty file");
    do {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue ;
        std::vector<std::string> nocom = checkComment(ft_split(line, WS));
        for (std::vector<std::string>::iterator it = nocom.begin(); it != nocom.end(); it++)
            file = file + *it + ' ';
    } while (std::getline(filename, line));
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
    return (file);
}

// Function to crop the config in the brackets (location or server)
// Returns the cropped part without brackets and trims this part from the buf
std::string   Parse::blockCrop(std::string &buf)
{
    size_t pos = buf.find('{');
    if (pos == std::string::npos)
        throw std::invalid_argument("Configuration file error: missing bracket after a keyword: " + buf.substr(0, buf.find('\n')));
    if (ft_split(buf.substr(0, pos), WS).size() != 1)
        throw std::invalid_argument("Configuration file error: wrong amout of arguments: " + buf.substr(0, buf.find('{')));

    buf = buf.substr(pos + 1);
    pos = 0;
    int br = 1;
    while (br > 0 && buf[pos])
    {
        if (buf[pos] == '{')
            br++;
        if (buf[pos] == '}')
            br--;
        pos++;
    }
    if (br)
        throw std::invalid_argument("Configuration file error: missing brackets");
    
    std::string   complexDir = buf.substr(0, pos - 1);
    buf = buf.substr(pos);
    return (complexDir);
}

std::vector<std::string>  Parse::checkComment(std::vector<std::string> line)
{
    std::vector<std::string> newvec;

    for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); it++)
    {
        if (it->empty())
            continue ;
        else if ((*it)[0] == '#')
            break ;
        else
            newvec.push_back(*it);
    }
    return (newvec);
}


// OVERLOADED OPERATORS ______________________________________

std::ostream	&operator<<(std::ostream &out, const std::vector<std::string> &val)
{
    for (unsigned int i = 0; i < val.size(); i++)
		out << val[i] << "  "; 
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::vector<int> &val)
{
    for (unsigned int i = 0; i < val.size(); i++)
		out << val[i] << "  ";
	return (out);
}

std::string	 printBool(const bool val)
{
    if (!val)
		return ("false ");
    else
        return ("true ");
}

std::ostream	&operator<<(std::ostream &out, const std::map<std::string, std::string> &valinit)
{
    std::map<std::string, std::string> val = valinit;
    for (std::map<std::string, std::string>::iterator it = val.begin(); it != val.end(); it++)
		out << "   [\"" + it->first + "\"] = " + it->second << "\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const std::map<int, std::pair<std::string, std::string> > &valinit)
{
	std::map<int, std::pair<std::string, std::string> > val = valinit;

    for (std::map<int, std::pair<std::string, std::string> >::iterator it = val.begin(); it != val.end(); it++)
		out << "   [\"" << it->first << "\"] = " + it->second.second << "\n";
	return (out);
}

std::ostream	&operator<<(std::ostream &out, const ServerConfig &val)
{
    out << "Port:  " << val.getPort() << "\n";
    out << "Host:  " << val.getHost() << "\n";
    out << "IP:  " << val.getIp() << "\n";
    out << "Server names:  " << val.getServerName() << "\n";
    out << "Root:  " << val.getRoot() << "\n";
    out << "Max Body Size:  " << val.getMaxBodySize() << "\n";
    out << "Autoindex:  " << printBool(val.getAutoIndex()) << "\n";
    out << "Allowed methods:  " << val.getAllowedMethods() << "\n";
//  out << "Error pages:  \n" << val.getErrorPages() << "\n";
//  out << "Error page empty:  " << printBool(val.getErrorPages().empty()) << "\n";
    out << "    CGI:  " << "\n" << val.getCgiConf() << "\n";
    std::vector<LocationConfig> temp = val.getLocationConfig();
    for (std::vector<LocationConfig>::iterator it = temp.begin(); it != temp.end(); it++) {
        out << "    LOCATION \n" << (*it) << "\n";
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
    out << "    |___    Allow upload:  " << printBool(val.getAllowUpload()) << "\n";
    out << "    |___    Autoindex:  " << printBool(val.getAutoIndex()) << "\n";
    out << "    |___    Allowed methods:  " << val.getAllowedMethods() << "\n";
 //   out << "    |___    Error pages:  \n         " << val.getErrorPages() << "\n";
    out << "    |______     CGI:  " << "\n" << val.getCgiConf() << "\n";

	return (out);
}

// TEMPLATE PROTOTYPES ______________________________________

template void Parse::complexParse<ServerConfig>(ServerConfig &serv, std::string &block);
template void Parse::complexParse<LocationConfig>(LocationConfig &serv, std::string &block);
template void Parse::lineParse<ServerConfig>(ServerConfig &serv, std::vector<std::string> args);
template void Parse::lineParse<LocationConfig>(LocationConfig &serv, std::vector<std::string> args);
template void Parse::blockParse<ServerConfig>(ServerConfig &serv, std::string &line);
template void Parse::blockParse<LocationConfig>(LocationConfig &serv, std::string &line);
