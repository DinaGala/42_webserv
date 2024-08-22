/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:35 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/19 15:21:08 by nuferron         ###   ########.fr       */
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

		/*	GETTERS	*/

		const std::string 					getHost() const;
		const std::string 					getIp() const;
		const std::vector<int>				getPort() const;
		const std::vector<std::string>		getServerName() const;
		const std::string			 		getRoot() const;
		const std::vector<LocationConfig>	getLocationConfig() const;
		size_t 								getMaxBodySize() const;
		bool 								getAutoIndex() const;
		const std::vector<std::string> 		getAllowedMethods() const;
		const std::map<std::string, func>& 	getKeys();
		const std::map<std::string, bool>& 	getVars();
		const std::map<std::string, std::string> 					getCgiConf() const;
		const std::map<int, std::pair<std::string, std::string> > 	getErrorPages() const;

		/*	SETTERS	*/

		void 		setHost(const std::string& host);
		void 		setIp(const std::string& ip);
		void 		setPort(int port);
		void 		unsetPort(int port);
		void 		setServerName(const std::string& serverName);
		void 		setRoot(const std::string& root);
		void 		setClientMaxBodySize(size_t client_max_body_size);
		void 		setCgiConf(const std::string &ext, const std::string &path);
		void 		setAutoIndex(bool autoIndex);
		void 		setHostName(const std::string& hostName);
		void 		setLocationConfig(const LocationConfig& location);
		void 		setErrorPage(int code, const std::string& page);
		void 		setAllowMethod(const std::string& method);
		void 		setVars(const std::string& key);
		const bool	loc;

	private:
		std::map<int, std::pair<std::string, std::string> >	_errorPages;
		static std::map<int, std::pair<std::string, std::string> >	_initStatus();
		std::vector<int> 					_port;
		std::string 						_host;
		std::string							_ip;
		std::vector<std::string> 			_serverName;
		std::string 						_root; 
		size_t 								_maxBodySize; 
		bool 								_autoIndex;
		std::vector<LocationConfig> 		_locations; 
		std::vector<std::string> 			_allowedMethods;
		std::map<std::string, std::string> 	_cgiConf;
		std::map<std::string, func>			_keys;
		std::map<std::string, bool>			_vars; 
		void								_initKeys();
		ServerConfig();
};

#endif
