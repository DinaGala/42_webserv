/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:38:05 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/21 20:59:03 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"



int	main(int ac, char **av)
{
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