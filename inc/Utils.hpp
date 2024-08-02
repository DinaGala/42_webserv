/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:44 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/01 21:26:02 by nuferron         ###   ########.fr       */
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
# define BUFFER_SIZE 1024
# define WS " \v\t\n\r"
# define NUM "0123456789"
# define TIMEOUT 600000000 // in seconds

# include <string>
# include <cstring>
# include <fstream>
# include <sstream>
# include <iostream>
# include <vector>
# include <map>
# include <list>
# include <algorithm>
# include <exception>
# include <cctype>
# include <functional>
# include <climits>
# include <fcntl.h>
# include <sys/epoll.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <netdb.h>
# include <netinet/in.h>
# include <cstdlib> // For exit() and EXIT_FAILURE 
# include <unistd.h>
# include <cerrno>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sstream>
# include <stdexcept> // For standard exceptions


# include "Response.hpp"
# include "Request.hpp"
# include "LocationConfig.hpp"
# include "ServerConfig.hpp"
# include "Parse.hpp"
# include "Server.hpp"
# include "Cgi.hpp"
# include "Socket.hpp"
# include "Cluster.hpp"
# include "Signals.hpp"

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);


std::vector<std::string>  ft_split(std::string str, std::string del); 
int ft_atopi(const std::string str);
int ft_atoi(const std::string str);
std::string	isLetDig();
std::string ft_itoa(int nb);
bool isStringOfDigits(std::string line);
uint64_t strToHex(std::string line);

void	copyMap(std::map<int, std::pair<std::string, std::string> > &res, const std::map<int, std::pair<std::string, std::string> > &val);
std::string	ft_strstr(const std::string &h, const std::string &n);

#endif
