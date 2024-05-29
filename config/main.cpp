/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:33:23 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/05/29 18:33:55 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

void checkExtConf(char *filename)
{
	std::string	ext = ".conf";
	std::string	file(filename);
	std::string	name = file.substr()
	if 
}

std::vector<ServerConfig>	configParse(void *file)
{
	checkExtConf(file);
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