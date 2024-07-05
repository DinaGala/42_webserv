/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:38:05 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/05 14:41:55 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"



int	main(int ac, char **av)
{
	try
	{
		if (ac == 2) {
            std::vector<ServerConfig>	sconf = Parse::configParse(av[1]);
		//	std::cout << sconf;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'; // unable to connect error ()
	}
	return (0);
}