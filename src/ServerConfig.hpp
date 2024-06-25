/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/25 21:50:24 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "Utils.hpp"

# define MAX_BODY_SIZE 2000000000 //2GB
# define DEFAULT_BODY_SIZE 10000000 //10MB
# define N_SERV_DIR 9

class ServerConfig
{
	private:

		std::vector<int> 					_port; // Choose the port and host of each ’server’.		
		std::string 						_host; // Choose the port and host of each ’server’.
		std::string 						_hostName; // Choose the port and host of each ’server’.
		std::vector<std::string> 			_serverName; // Setup the server_names or not.
		std::string 						_root; // If empty it setups to default
		size_t 								_maxBodySize; // Limit client body size
		bool 								_autoIndex;
		std::map<int, std::string> 			_errorPages; // The string is the path. Setup default error pages.
		std::vector<LocationConfig> 		_locations; // Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
		std::vector<std::string> 			_allowedMethods; // Define a list of accepted HTTP methods for the route.
		std::map<std::string, std::string> 	_cgiConf;
//		std::map<std::string, bool>			_vars; // each variable if is set or not
		static std::map<std::string, funcS>	_keys;
		std::string							_key[N_SERV_DIR] = 
		{
			"listen",
    		"server_name",
    		"root",
   		 	"max_body_size",
    		"autoindex",
    		"error_pages",
    		"location",
    		"allow_methods",
    		"cgi"
		};
		
		ServerConfig();

	public:

		ServerConfig& operator=(const ServerConfig& src);
		ServerConfig(const ServerConfig& src);
		ServerConfig(std::string file);
		~ServerConfig();

		std::string 				getHost() const;
		std::vector<int>			getPort() const;
		std::vector<std::string>	getServerName() const;
		std::string			 		getRoot() const;
		std::vector<LocationConfig> getLocationConfig() const;
		std::map<int, std::string> 	getErrorPages() const;
		size_t 						getMaxBodySize() const;
		bool 						getAutoIndex() const;
		std::string 				getHostName() const;
		std::map<std::string, std::string> 	getCgiConf() const;

		void 		setHost(const std::string& host);
		void 		setPort(int port);
		void 		setServerName(const std::string& serverName);
		void 		setRoot(const std::string& root);
//		void 		setErrorPages(const std::map<int, std::string>& error_pages);
		void 		setClientMaxBodySize(size_t client_max_body_size);
//		void 		setCgiConf(const std::map<std::string, std::string>& cgiConf);
		void 		setCgiConf(const std::string &ext, const std::string &path);
		void 		setAutoIndex(bool autoIndex);
		void 		setHostName(const std::string& hostName);
		void 		setLocationConfig(const LocationConfig& location);
		void 		setErrorPage(int code, const std::string& page);
		void 		setAllowedMethod(const std::string& method);
		const bool	loc = true;
//		bool		empty;
};



#endif