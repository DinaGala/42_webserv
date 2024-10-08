/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/22 12:04:26 by nuferron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
# define UTILS_HPP

# define MAX_BODY_SIZE 2000000000 //2GB
# define DEFAULT_BODY_SIZE 10000000 //10MB
# define N_SERV_DIR 9
# define MAX_CON 10
# define MAX_EVENTS 20
# define BUFFER_SIZE 200000 
# define WS " \v\t\n\r"
# define NUM "0123456789"
# define TIMEOUT 5 // in seconds

# include <algorithm>
# include <cctype>
# include <climits>
# include <cstring>
# include <exception>
# include <fcntl.h>
# include <fstream>
# include <functional>
# include <iostream>
# include <list>
# include <map>
# include <sstream>
# include <stdlib.h>
# include <sys/epoll.h>
# include <unistd.h>
# include <vector>

# include <arpa/inet.h>
# include <cerrno>
# include <cstdlib> 
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <stdexcept> 
# include <sys/socket.h> 
# include <sys/types.h>

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);

std::vector<std::string>  ft_split(std::string str, std::string del); 

int         ft_atopi(const std::string str);
int         ft_atoi(const std::string str);
std::string ft_itoa(int nb);

std::string	isLetDig();
bool        isStringOfDigits(std::string line);
uint64_t    strToHex(std::string line);

void	    copyMap(std::map<int, std::pair<std::string, std::string> > &res, const std::map<int, std::pair<std::string, std::string> > &val);
std::string	ft_strstr(const std::string &h, const std::string &n);

#endif
