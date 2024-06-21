/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:13:39 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/21 20:17:17 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// Helper function to trim leading whitespace
std::string ltrim(const std::string& s) 
{
    std::string result = s;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return (result);
}

// Helper function to trim trailing whitespace
std::string rtrim(const std::string& s) 
{
    std::string result = s;
    result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
    return (result);
}

// Function to trim both leading and trailing whitespace
std::string trim(const std::string& s) 
{
    return (ltrim(rtrim(s)));
}