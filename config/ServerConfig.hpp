/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/05/29 18:25:01 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
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
		std::string _host; // Choose the port and host of each ’server’.
		std::string _hostName;
		int _port; // Choose the port and host of each ’server’.
		std::string _serverName; // Setup the server_names or not.
		std::string _rootDir;
		std::vector<LocationConfig> _locations; // Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
		std::map<int, std::string> error_pages; // What's in the string? Setup default error pages.
		size_t _max_body_size; // Limit client body size.
		std::vector<CgiConfig> cgiConf;
		bool autoindex;

		ServerConfig();

	public:

		ServerConfig& operator=(const ServerConfig& src);
		ServerConfig(const ServerConfig& src);
	//	ServerConfig(const std::string& host, int port, const std::string& serverName, const std::string& rootDir, const std::vector<LocationConfig>& locations, const std::map<int, std::string>& error_pages, size_t client_max_body_size, const std::vector<CgiConfig>& cgiConf, bool autoindex);
		ServerConfig(char *file);
		~ServerConfig();

		void _initConfig();
		std::string getHost() const;
		int getPort() const;
		std::string getServerName() const;
		std::string getRootDir() const;
		std::vector<LocationConfig> getLocationConf() const;
		std::map<int, std::string> getErrorPages() const;
		size_t getMaxBodySize() const;
		std::vector<CgiConfig> getCgiConf() const;
		bool getAutoIndex() const;
		void setHost(const std::string& host);
		void setPort(int port);
		void setServerName(const std::string& serverName);
		void setRootDir(const std::string& rootDir);
		void addLocationConfig(const LocationConfig& locations);
		void addErrorPage(int code, const std::string& page);
		void setErrorPages(const std::map<int, std::string>& error_pages);
		void setClientMaxBodySize(size_t client_max_body_size);
		void setCgiConf(const std::vector<CgiConfig>& cgiConf);
		void addCgiConfig(const CgiConfig& cgiConf);
		void setAutoIndex(bool autoindex);
		void setHostName(const std::string& hostName);
		std::string getHostName() const;
};

std::vector<ServerConfig>	configParse(void *file);

#endif