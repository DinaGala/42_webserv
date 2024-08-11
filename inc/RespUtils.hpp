#ifndef RESPUTILS_HPP
# define RESPUTILS_HPP

# define AUTOINDEX(content) \
	std::string("<html><head><title>AUTOINDEX</title>" \
	"<h1 style=\"text-align:center;font-size:200%;\">Index of ") + content \
	+ std::string("</h1></head><body style=\"font-size:150%;margin:50px;\">")

# define AUTOINDEX_FILES(href, name) \
	std::string("<p><a href= \'") + href + std::string("\'>") + name \
	+ std::string("</a></p>\n")

# define SEV_ERR "HTTP/1.1 505 Severe Internal Server Error\r\n" \
				"Content-Type: text/plain\r\nContent-Length: 47\r\n\r\n" \
				"Severe Internal Error.\nPlease, try again later\n"

# define WORK_DONE(str) \
	std::string("<html><head><title>As you wish</title><style>\n" \
	"body{\ndisplay: flex;\njustify-content: center;\n" \
	"align-items: center; height: 100vh;\n}\n" \
	".ctn {\ntext-align: center\n}\n" \
	".ctn h1{\nmargin: 1em 0}\n" \
	".ctn a{\ndisplay: inline-block;\nborder: none;\nborder-radius: 0.25em;\n" \
	"padding: 0.5em 1em;\ncolor: 330066;\n" \
	"background-color: #9966CC\n}</style><body><div class=\"ctn\"><h1>") + str + \
	std::string("</h1><a href=default.html>Go to Homepage</a></body></html>")

#endif
