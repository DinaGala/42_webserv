/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/17 18:47:52 by nzhuzhle         ###   ########.fr       */
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

# define MAX_BODY_SIZE 10000000000 //10GB
# define DEFAULT_BODY_SIZE 10000000 //10MB

class ServerConfig
{
	private:

		int 						_port; // Choose the port and host of each ’server’.		
		std::string 				_host; // Choose the port and host of each ’server’.
	//	std::string 				_hostName; // don't think we need it
		std::string 				_serverName; // Setup the server_names or not.
		std::string 				_rootDir; // If empty it setups to default
		size_t 						_max_body_size; // Limit client body size
		bool 						_autoindex;
		std::vector<CgiConfig> 		_cgiConf;
		std::map<int, std::string> 	_error_pages; // The string is the path. Setup default error pages.
		std::vector<LocationConfig> _locations; // Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
		ServerConfig();

	public:

		ServerConfig& operator=(const ServerConfig& src);
		ServerConfig(const ServerConfig& src);
		ServerConfig(std::ifstream &file);
		~ServerConfig();

		void 						_initConfig();

		std::string 				getHost() const;
		int 						getPort() const;
		std::string 				getServerName() const;
		std::string			 		getRootDir() const;
		std::vector<LocationConfig> getLocationConf() const;
		std::map<int, std::string> 	getErrorPages() const;
		size_t 						getMaxBodySize() const;
		std::vector<CgiConfig> 		getCgiConf() const;
		bool 						getAutoIndex() const;
		std::string 				getHostName() const;

		void 		setHost(const std::string& host);
		void 		setPort(int port);
		void 		setServerName(const std::string& serverName);
		void 		setRootDir(const std::string& rootDir);
		void 		setErrorPages(const std::map<int, std::string>& error_pages);
		void 		setClientMaxBodySize(size_t client_max_body_size);
		void 		setCgiConf(const std::vector<CgiConfig>& cgiConf);
		void 		addCgiConfig(const CgiConfig& cgiConf);
		void 		setAutoIndex(bool autoindex);
		void 		setHostName(const std::string& hostName);
		
		void 		addLocationConfig(const LocationConfig& locations);
		void 		addErrorPage(int code, const std::string& page);
		bool		empty;
};

std::vector<ServerConfig>	configParse(void *file);

#endif