/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzhuzhle <nzhuzhle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:02:04 by nzhuzhle          #+#    #+#             */
/*   Updated: 2024/05/28 18:03:30 by nzhuzhle         ###   ########.fr       */
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
	CgiConfig(const std::string& ext, const std::string& handler)
    : extension(ext), handlerPath(handler) {}
};

class LocationConfig
{
	private:
		std::string _uri;
		std::string _root;
		std::string _alias;
		std::map<int, std::string> _error_pages;
		std::vector<std::string> _allowed_methods;
		std::string _index;
		bool _autoIndex;
		bool _allowUpload;
		std::string _uploadDir;
		std::vector<CgiConfig> _cgiConfigs;

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

		const std::string& getUri() const;
		const std::string& getRoot() const;
		const std::string& getAlias() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::vector<std::string>& getAllowedMethods() const;
		const std::vector<CgiConfig>& getCgiConfigs() const;
		const std::string& getIndex() const;
		bool getAutoIndex() const;
		bool getAllowUpload() const;
		const std::string& getUploadDir() const;
	
};
#endif







