#include "Cgi.hpp"

Cgi::Cgi(const std::string &serv, int port, const std::string &method,
		const std::string &url, const std::string &ctype, const std::string &clen)
{
	this->_pairs[".php"] = "usr/bin/php";
	this->_pairs[".js"] = "usr/bin/node";
	this->_pairs[".sh"] = "/bin/bash";
	this->_pairs[".py"] = "usr/bin/python3";
	this->_env.push_back("SERVER_SOFTWARE=webserver");
	this->_env.push_back("GATTEWAY_INTERFACE=CGI/1.1");
	this->_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	this->_env.push_back("SERVER_NAME=" + serv);
	this->_env.push_back("REQUEST_METHOD=" + method);
	this->_env.push_back("CONTENT_TYPE=" + ctype);
	this->_env.push_back("CONTENT_LENGTH=" + clen);
	std::ostringstream oss;
	oss << port;
	this->_env.push_back("SERVER_PORT=" + oss.str());
}

Cgi::Cgi(const Cgi &c)
{
	(void)c;
}

Cgi::~Cgi() {}

Cgi	&Cgi::operator=(const Cgi &c)
{
	(void)c;
	return (*this);
}

void	Cgi::addPair(std::string &ext, std::string &cmd)
{
	this->_pairs[ext] = cmd;
}

char	**Cgi::vecToMat(std::vector<string> &vec)
{
	char	**mat;
	size_t	len = vec.size();

	try
	{
		mat = new char *[len + 1];
	}
	catch (std::exception &ex)
	{
		std::cerr << ex << std::endl;
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < len; i++)
	{
		try
		{
			mat[i] = new char [vec[i].length + 1];
			std::strcpy(mat[i], vec[i].c_str());
		}
		catch (std::exception &ex)
		{
			std::cerr << ex << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return (mat);
}

char	**Cgi::getEnv(void)
{
	return (this->vecToMat(this->_env));
}

