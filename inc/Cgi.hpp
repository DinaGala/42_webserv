#ifndef	CGI_HPP
# define CGI_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

class	Cgi
{
	public:
		Cgi();
		~Cgi();
		void	addPair(std::string &ext, std::string &cmd);

	private:
		std::map<std::string, std::string>	_pairs;
		std::vector<std::string>			_env;

		Cgi(const Cgi &c);
		Cgi		&operator=(const Cgi &c);
		char	**vecToMat(std::vector<string> &vec);
};

#endif
