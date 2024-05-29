/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:33:23 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/05/29 22:26:07 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

// void checkExtConf(char *filename)
// {
// 	std::string	ext = ".conf";
// 	std::string	file(filename);
// 	std::string	end = file.substr(file.length() - ext.length());

// 	if (file.length() < 6 || )
// }

std::vector<ServerConfig>	configParse(std::string &filename)
{
//	checkExtConf(file);
	
	std::ifstream				file(filename);
	std::vector<ServerConfig>	sconf;
	std::string					buf;

	if (!file.is_open())
		throw std::invalid_argument("Error when opening config file.");
	while (24)
	{
		ServerConfig	newserv(file);
		if (newserv.empty)
			break ;
		sconf.push_back(newserv);
	}
	return (sconf);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{
		std::vector<ServerConfig>	sconf = configParse(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'; // unable to connect error ()
	}

	return (0);
}