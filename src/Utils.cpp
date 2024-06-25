/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:39 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/25 21:07:54 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

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

template <typename Container>
Container&   ft_split(std::string str, char c)
{
    Container  res;
    size_t  start = 0;
    size_t  pos = str.find(c);
    while (end != std::string::npos)
    {
        res.push_back(str.substr(start, end - start));
        start = end + 1;
        end = std.find(c, start);
    }
    res.push_back(str.substr(start));
    return (res);
}