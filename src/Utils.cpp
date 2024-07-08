/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:39 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/07/08 15:57:49 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

// Helper function to trim leading whitespace
std::string ltrim(const std::string& str) 
{
    size_t pos = str.find_first_not_of(" \v\t\n\r");

    if (pos == std::string::npos) 
        return ("");
    return (str.substr(pos));
}

// Helper function to trim trailing whitespace
std::string rtrim(const std::string& str) 
{
    size_t pos = str.find_last_not_of(" \v\t\n\r");
    return (str.substr(0, pos + 1));
}

// Function to trim both leading and trailing whitespace
std::string trim(const std::string& s) 
{
    return (ltrim(rtrim(s)));
}


std::vector<std::string>   ft_split(std::string s, std::string del)
{
    std::vector<std::string>  res;
    std::string str = trim(s);

    if (str.empty())
        return (res);
    size_t  start = 0;
    size_t  end = str.find_first_of(del);
    while (end != std::string::npos)
    {
        if (!str.substr(start, end - start).empty())
            res.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find_first_of(del, start);
    }
    res.push_back(str.substr(start));
    return (res);
}

/* A function that converts a string into a positive integer,
or throws an exception otherwise */
int 	ft_atopi(const std::string str)
{
	if (str.empty())
		throw std::invalid_argument("Error: empty string cannot be converted into a positive integer");
	for (int i = 0; str[i]; i++)
	{
		if (i > 10 || !isdigit(str[i]))
			throw std::invalid_argument("Error: not a positive integer: " + str);
	}
	if (atol(str.c_str()) > INT_MAX)
		throw std::invalid_argument("Error: not a positive integer: " + str);
	return (atol(str.c_str()));
}

/* A function that converts a string into an integer,
or throws an exception otherwise */
int 	ft_atoi(const std::string str)
{
	if (str.empty())
		throw std::invalid_argument("Error: empty string cannot be converted into an integer");
	for (int i = 0; str[i]; i++)
	{
		if (i == 0 && str[i] == '-')
            continue ;
        else if (i > 11 || !isdigit(str[i]))
			throw std::invalid_argument("Error: not an integer: " + str);
	}
	if (atol(str.c_str()) > INT_MAX || atol(str.c_str()) < INT_MIN)
		throw std::invalid_argument("Error: not an integer: " + str);
	return (atol(str.c_str()));
}

/* returns a string of letters and digits */
std::string	isLetDig()
{
    return ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
}
