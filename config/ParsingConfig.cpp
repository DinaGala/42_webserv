/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:44:49 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/20 20:35:13 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsingConfig.hpp"

std::string &checkBrackets(std::ifstream &filename)
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

std::vector<ServerConfig>	configParse(std::string &filename)
{
	std::ifstream				file(filename);
	std::vector<ServerConfig>	sconf;
	std::string					buf;

	if (!file.is_open())
		throw std::invalid_argument("Error when opening config file.");
    buf = checkBrackets(file);
    while (24)
	{
		buf = ltrim(buf);
        if (buf.substr(0, 6) != "server")
            throw std::invalid_argument("Configuration file error: no server keyword: " + buf.substr(0, buf.find('\n')));
        ServerConfig	newserv(cropComplexDir(buf));
		if (newserv.empty)
			break ;
		sconf.push_back(newserv);
	}
	return (sconf);
}

// Function to crop the config in the brackets (location or server)
// Returns the cropped part without brackets and trims this part from the buf
std::stringstream   &cropComplexDir(std::string &buf)
{
    size_t pos = buf.find('{');
    if (pos == std::string::npos)
        throw std::invalid_argument("Configuration file error: missig bracket after a keyword: " + buf.substr(0, buf.find('\n')));
    buf = buf.substr(0, pos + 1);
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
        throw std::invalid_argument("Configuration file error: missig brackets");

    std::stringstream   complexDir(buf.substr(0, pos));
    buf = buf.substr(pos);
    return (complexDir);
}

// Helper function to trim leading whitespace
std::string ltrim(const std::string& s) 
{
    std::string result = s;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return (result);
}

// Helper function to trim trailing whitespace
std::string rtrim(const std::string& s) 
{
    std::string result = s;
    result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
    return (result);
}

// Function to trim both leading and trailing whitespace
std::string trim(const std::string& s) 
{
    return (ltrim(rtrim(s)));
}