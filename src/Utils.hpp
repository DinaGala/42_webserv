/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/21 20:19:27 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <cstring>
# include <fstream>
# include <sstream>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <exception>
# include <cctype>


# include "LocationConfig.hpp"
# include "ServerConfig.hpp"
# include "ParsingConfig.hpp"

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);



#endif