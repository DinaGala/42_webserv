#include "Request.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

Request::Request() {	
}

Request::~Request() {
}

/*	Request Line: Contains the HTTP method, the URL, and the HTTP version.
	Headers: A series of key-value pairs, each on its own line.
	Blank Line: A line with no content, indicating the end of the headers.
	Body: Optional, depending on the type of request (e.g., present in POST requests).*/


bool	Request::parseRequest(const std::string & buffer) {

	std::cout << "buffer:\n" << buffer << std:: endl << std::endl;
	size_t endRequestLine = buffer.find('\n');
	if (endRequestLine == std::string::npos) {
		std::cout << "no \n in request line" << std:: endl;
		return (false); // TODO: manage exceptions
	}
	if (!this->parseRequestLine(buffer.substr(0, endRequestLine))) {
		std::cout << "Request line wrong 1" << std::endl;
		return (false); // TODO: manage exceptions
	}

	size_t endHeaders = buffer.find('\r', endRequestLine);
	if (endHeaders == std::string::npos) {
		std::cout << "no \r in headers" << std:: endl;
		return (false); // TODO: manage exceptions
	}
	if (!this->parseHeaders(buffer.substr(endRequestLine + 1, endHeaders))) {
		std::cout << "header wrong" << std:: endl;
		return (false); // TODO: manage exceptions
	}
 
	//TODO: PARSING BODY

	return (true);
}

bool	Request::parseRequestLine(const std::string & requestLineStr) {
	fillRequestLineVector(requestLineStr);
	if (_requestLine.size() != 3) {
		std::cout << "there is not 3 element in request line" << std::endl;
		return (false); // TODO: manage exceptions
	}

	for (std::vector<std::string>::iterator it = _requestLine.begin(); it != _requestLine.end(); it++) {
		std::cout << "word: " << *it << std::endl;
	}
	
	std::vector<std::string> requestVect = createValidRequestVector();

	std::vector<std::string>::iterator it = std::find(requestVect.begin(), requestVect.end(), _requestLine.front()); //
	if (it == requestVect.end()) {
		std::cout << "wrong request method";
		return false; // TODO: manage exceptions
	}

	//TODO PARSE ROOT _firstLine ??

	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1")) {
		std::cout << "bad http version" << std:: endl;
		return false; // TODO: manage exceptions
	}
	return (true); 
}

void Request::fillRequestLineVector(const std::string & requestLineStr){
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
	}
}

std::vector<std::string>  Request::createValidRequestVector(){
	std::vector<std::string> requestVect;
	requestVect.push_back("GET");
	requestVect.push_back("POST");
	requestVect.push_back("DELETE");
	return requestVect;
}

bool	Request::parseHeaders(const std::string &header) {
	
	std::cout << "HEADER" << std::endl;
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
	}
	return (true);
}


