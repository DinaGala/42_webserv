/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:04 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/17 18:33:18 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <iostream>
# include <vector>
# include <map>

struct CgiConfig 
{
    std::string extension;
    std::string handlerPath;
	CgiConfig(const std::string& ext, const std::string& handler): extension(ext), handlerPath(handler) {}
};

class LocationConfig
{
	private:
		std::string 				_uri; 
		std::string 				_root; // Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
		std::string 				_alias;
		std::string 				_index;
		bool 						_autoIndex;
		bool 						_allowUpload;
		std::string 				_uploadDir;
		std::vector<CgiConfig> 		_cgiConfigs; // Execute CGI based on certain file extension (for example .php).
		std::vector<std::string> 	_allowed_methods; // Define a list of accepted HTTP methods for the route.
		std::map<int, std::string> 	_error_pages;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig& src);
		LocationConfig& operator=(const LocationConfig& src);
		~LocationConfig();

		void setUri(const std::string& uri);
		void setRoot(const std::string& root);
		void setAlias(const std::string& alias);
		void setErrorPages(const std::map<int, std::string>& error_pages);
		void setAllowedMethods(const std::vector<std::string>& allowed_methods);
		void addCgiConfig(const CgiConfig& cgiConfig);
		void setIndex(const std::string& index);
		void setAutoIndex(bool autoIndex);
		void setAllowUpload(bool allowUpload);
		void setUploadDir(const std::string& uploadDir);

		bool getAutoIndex() const;
		bool getAllowUpload() const;
		const std::string& 					getUri() const;
		const std::string& 					getRoot() const;
		const std::string& 					getAlias() const;
		const std::string& 					getIndex() const;
		const std::string& 					getUploadDir() const;
		const std::vector<CgiConfig>& 		getCgiConfigs() const;
		const std::vector<std::string>& 	getAllowedMethods() const;
		const std::map<int, std::string>& 	getErrorPages() const;
};
#endif







