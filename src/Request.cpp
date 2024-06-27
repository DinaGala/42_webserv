#include "Request.hpp"


Request::Request() {	
}

Request::~Request() {
}

/*	Request Line: Contains the HTTP method, the URL, and the HTTP version.
	Headers: A series of key-value pairs, each on its own line.
	Blank Line: A line with no content, indicating the end of the headers.
	Body: Optional, depending on the type of request (e.g., present in POST requests).*/


void	Request::parseRequest(const std::string & buffer) {

	try {
		std::cout << "buffer:\n" << buffer << std:: endl << std::endl;
		parseRequestLine(buffer);
		parseHeaders(buffer);
		//TODO: PARSING BODY
	} catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
	}

}

void	Request::parseRequestLine(const std::string & buffer) {
	
	size_t endRequestLine = buffer.find('\n');
	
	if (endRequestLine == std::string::npos) {
		throw std::runtime_error("Error parsing Request: no \n in request");
	}

	std::string requestLineStr = buffer.substr(0, endRequestLine);
	fillRequestLineVector(requestLineStr);
	if (_requestLine.size() != 3) {
		throw std::runtime_error("Error parsing Request: wrong request line");
	}
	std::vector<std::string> requestVect = createValidRequestVector(); //TODO: use ServerConfig
	std::vector<std::string>::iterator it = std::find(requestVect.begin(), requestVect.end(), _requestLine.front()); //
	if (it == requestVect.end()) {
		throw std::runtime_error("Error parsing Request: no metod allowed");
	}

	//TODO: PARSE ROOT _firstLine ??

	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1")) {
		throw std::runtime_error("Error parsing Request: bat http version");
	}
}

void Request::fillRequestLineVector(std::string requestLineStr){
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
	}
}

//Pending to use configFile method allowed vector
std::vector<std::string>  Request::createValidRequestVector(){
	std::vector<std::string> requestVect;
	requestVect.push_back("GET");
	requestVect.push_back("POST");
	requestVect.push_back("DELETE");
	return requestVect;
}

void	Request::parseHeaders(const std::string &header) {
	
	/*std::cout << "HEADER" << std::endl;
	size_t posStartLine = 0;
	size_t posEndLine = header.find('\n');;
	size_t posColon = header.find(':');

	while (posEndLine != std::string::npos) {
		std::string name = "";
		std::string value = "";
		if (posColon == std::string::npos || posColon > posEndLine){
			std::cout << "Bad headers" << std:: endl;
			return false; // TODO: manage exceptions
		}
		name = header.substr(posStartLine, posColon - posStartLine);
        value = header.substr(posColon + 2, posEndLine - posColon -2);
		_headers.insert(std::make_pair(name, value));

		posStartLine = posEndLine + 1;
		posColon = header.find(':', posEndLine + 1);
		posEndLine = header.find('\n', posEndLine + 1);
	}
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
}


