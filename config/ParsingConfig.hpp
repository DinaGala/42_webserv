/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/20 20:30:44 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

# include "LocationConfig.hpp"
# include "ServerConfig.hpp"
# include <string>
# include <fstream>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <exception>
# include <cctype>
# include <sstream>

class ServerConfig;
class LocationConfig;

template <typename T>
void  errorPagesParse(T &obj, std::vector<int> &err, std::string &path);



std::vector<ServerConfig>	configParse(std::string &file);

std::string &checkBrackets(std::ifstream &filename);
std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);
// void        exceptionFormat()

#endif