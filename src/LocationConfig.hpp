/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:04 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/06/21 20:20:26 by nzhuzhle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include "Utils.hpp"

# define N_LOC_DIR 9

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
		std::vector<std::string> 	_allowedMethods; // Define a list of accepted HTTP methods for the route.
		std::map<int, std::string> 	_errorPages;
		std::map<std::string, std::string> 	_cgiConf; // Execute CGI based on certain file extension (for example .php).
		std::map<std::string, funcs *>	_keys;
		std::string							_keys[N_LOC_DIR] = 
		{
    		"root",
    		"autoindex",
    		"error_pages",
			"upload_dir",
			"allow_upload",
			"index",
    		"allow_methods",
			"return",
    		"cgi"
		};

		void	_initKeys();
		LocationConfig();

	public:
		LocationConfig(std::string &file);
		LocationConfig(const LocationConfig& src);
		LocationConfig& operator=(const LocationConfig& src);
		~LocationConfig();

		void setUri(const std::string& uri);
		void setRoot(const std::string& root);
		void setAlias(const std::string& alias);
		void setErrorPage(int code, const std::string& page);
		void setAllowedMethod(const std::string& method);
//		void addCgiConfig(const std::map<std::string, std::string>& cgiConfig);
		void setCgiConf(const std::string &ext, const std::string &path);
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
		const std::map<std::string, std::string>& 		getCgiConf() const;
		const std::vector<std::string>& 	getAllowedMethods() const;
		const std::map<int, std::string>& 	getErrorPages() const;
};
#endif
