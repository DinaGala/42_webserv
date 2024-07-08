/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:38:05 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/08 15:57:09 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"



int	main(int ac, char **av)
{
	try
	{
		if (ac == 2) {
            std::vector<ServerConfig>	sconf = Parse::configParse(av[1]);
		//	std::cout << sconf;
		}
		else if (ac == 1)
		{
            std::vector<ServerConfig>	sconf = Parse::configParse();
		//	std::cout << sconf;
		}
		else
			throw std::invalid_argument("Wrong amount of arguments: introduce only one config file or nothing");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'; // unable to connect error ()
	}
	return (0);
}