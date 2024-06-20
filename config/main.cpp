/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:38:05 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/20 19:16:06 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsingConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"



int	main(int ac, char **av)
{
	if (ac != 2)
	try
	{
		if (ac == 2)
            std::vector<ServerConfig>	sconf = configParse(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'; // unable to connect error ()
	}

	return (0);
}