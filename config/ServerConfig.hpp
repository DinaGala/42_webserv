/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/19 18:34:50 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <vector>
# include <map>
# include "LocationConfig.hpp"
# include <algorithm>

# define MAX_BODY_SIZE 2000000000 //2GB
# define DEFAULT_BODY_SIZE 10000000 //10MB

class ServerConfig
{
	private:

		std::vector<int> 					_port; // Choose the port and host of each ’server’.		
		std::string 						_host; // Choose the port and host of each ’server’.
		std::vector<std::string> 			_serverName; // Setup the server_names or not.
		std::string 						_rootDir; // If empty it setups to default
		size_t 								_max_body_size; // Limit client body size
		bool 								_autoindex;
		std::map<int, std::string> 			_error_pages; // The string is the path. Setup default error pages.
		std::vector<LocationConfig> 		_locations; // Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
		std::map<std::string, std::string> 	_cgiConf;
		std::map<std::string, bool>			_vars; // each variable if is set or not
		ServerConfig();

	public:

		ServerConfig& operator=(const ServerConfig& src);
		ServerConfig(const ServerConfig& src);
		ServerConfig(std::stringstream &file);
		~ServerConfig();

		void 						_initConfig();

		std::string 				getHost() const;
		std::vector<int>			getPort() const;
		std::vector<std::string>	getServerName() const;
		std::string			 		getRootDir() const;
		std::vector<LocationConfig> getLocationConf() const;
		std::map<int, std::string> 	getErrorPages() const;
		size_t 						getMaxBodySize() const;
		bool 						getAutoIndex() const;
	//	std::string 				getHostName() const;
		std::map<std::string, std::string> 	getCgiConf() const;

		void 		setHost(const std::string& host);
		void 		setPort(int port);
		void 		setServerName(const std::string& serverName);
		void 		setRootDir(const std::string& rootDir);
		void 		setErrorPages(const std::map<int, std::string>& error_pages);
		void 		setClientMaxBodySize(size_t client_max_body_size);
		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);
		void 		addCgiConfig(const std::map<std::string, std::string>& cgiConf);
		void 		setAutoIndex(bool autoindex);
		void 		setHostName(const std::string& hostName);
		
		void 		addLocationConfig(const LocationConfig& locations);
		void 		addErrorPage(int code, const std::string& page);
		bool		empty;
};



#endif