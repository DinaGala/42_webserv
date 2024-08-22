/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:04 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/08/22 12:07:26 by nuferron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include "Utils.hpp"
# include "Parse.hpp"

class Parse;

class LocationConfig
{
	public:
		LocationConfig(std::string url, std::map<int, std::pair<std::string, std::string> > err, std::string file);
		LocationConfig(const LocationConfig& src);
		LocationConfig& operator=(const LocationConfig& src);
		std::map<int, std::pair<std::string, std::string> >	operator=(const std::map<int, std::pair<std::string, std::string> > &val);
		~LocationConfig();

		typedef void (*func)(LocationConfig &, std::vector<std::string> &);
		bool getAutoIndex() const;
		bool getAllowUpload() const;
		const std::string& 					getUri() const;
		const std::string& 					getRoot() const;
		const std::string& 					getReturn() const;
		const std::string& 					getIndex() const;
		const std::string& 					getUploadDir() const;
		const std::vector<std::string>& 	getAllowedMethods() const;
		const std::map<int, std::pair<std::string, std::string> >& 	getErrorPages() const;
		const std::map<std::string, func>& 	getKeys() const;
		const std::map<std::string, std::string>&	getCgiConf() const;
		const std::map<std::string, bool>& 	getVars();
		
		void setUri(const std::string& uri);
		void setRoot(const std::string& root);
		void setReturn(const std::string& alias);
		void setErrorPage(int code, const std::string& page);
		void setAllowMethod(const std::string& method);
		void setCgiConf(const std::string &ext, const std::string &path);
		void setIndex(const std::string& index);
		void setAutoIndex(bool autoIndex);
		void setAllowUpload(bool allowUpload);
		void setUploadDir(const std::string& uploadDir);
		void setLocationConfig(const LocationConfig& location) {
			(void)location;
		}
		void setVars(const std::string& key);

		const bool	loc;

	private:
		std::string 						_uri; 
		std::string 						_root; 
		std::string 						_return;
		std::string 						_index;
		bool 								_autoIndex;
		bool 								_allowUpload;
		std::string 						_uploadDir;
		std::vector<std::string> 			_allowedMethods;
		std::map<int, std::pair<std::string, std::string> >	_errorPages;
		std::map<std::string, std::string> 	_cgiConf; 
		std::map<std::string, func>			_keys;
		std::map<std::string, bool>			_vars; 
		void								_initKeys();

		LocationConfig();
};
#endif
