/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:23:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/19 21:50:22 by nzhuzhle         ###   ########.fr       */
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

class ServerConfig;
class LocationConfig;

template <typename T>
void  errorPagesParse(T &obj, std::vector<int> &err, std::string &path);



std::vector<ServerConfig>	configParse(void *file);



#endif