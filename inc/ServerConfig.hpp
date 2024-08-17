/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/17 15:38:13 by nuferron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "Utils.hpp"

class Parse;
class LocationConfig;

class ServerConfig
{
	public:

		ServerConfig& operator=(const ServerConfig& src);
		std::map<int, std::pair<std::string, std::string> >	operator=(const std::map<int, std::pair<std::string, std::string> > &val);
		ServerConfig(const ServerConfig& src);
		ServerConfig(std::string file);
		~ServerConfig();

		typedef void (*func)(ServerConfig &, std::vector<std::string> &);
		const std::string 				getHost() const;
		const std::string 				getIp() const;
		const std::vector<int>			getPort() const;
		const std::vector<std::string>	getServerName() const;
		const std::string			 		getRoot() const;
		const std::vector<LocationConfig> getLocationConfig() const;
		const std::map<int, std::pair<std::string, std::string> > 	getErrorPages() const;
		size_t 						getMaxBodySize() const;
		bool 						getAutoIndex() const;
//		const std::string 				getHostName() const;
		const std::map<std::string, std::string> 	getCgiConf() const;
		const std::vector<std::string> 		getAllowedMethods() const;
		const std::map<std::string, func>& 	getKeys();
		const std::map<std::string, bool>& 	getVars();

		void 		setHost(const std::string& host);
		void 		setIp(const std::string& ip);
		void 		setPort(int port);
		void 		unsetPort(int port);
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
		void 		setAllowMethod(const std::string& method);
		void 		setVars(const std::string& key);
		const bool	loc;
//		bool		empty;

	private:
		static std::map<int, std::pair<std::string, std::string> >	_errorPages;
		static std::map<int, std::pair<std::string, std::string> >	_initStatus();
		std::vector<int> 					_port; // Choose the port and host of each ’server’.		
		std::string 						_host; // Choose the port and host of each ’server’.
		std::string							_ip;
//		std::string 						_hostName; // Choose the port and host of each ’server’.
		std::vector<std::string> 			_serverName; // Setup the server_names or not.
		std::string 						_root; // If empty it setups to default
		size_t 								_maxBodySize; // Limit client body size
		bool 								_autoIndex;
//		std::map<int, std::string> 			_errorPages; // The string is the path. Setup default error pages.
		std::vector<LocationConfig> 		_locations; // Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp)
		std::vector<std::string> 			_allowedMethods; // Define a list of accepted HTTP methods for the route.
		std::map<std::string, std::string> 	_cgiConf;
		std::map<std::string, func>			_keys;
		std::map<std::string, bool>			_vars; // each variable if is set or not
		void								_initKeys();
		ServerConfig();

};


#endif
