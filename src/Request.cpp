#include "Request.hpp"

Request::Request(const std::string & buffer) : _buffer(buffer), _status(INITIAL_STATUS), _body("") {
}

Request::~Request() {
}

/*	REQUEST LINE: method | URI and protocol | version
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/

void	Request::parseRequest() {
	try {
		
		std::cout << "buffer:\n" << _buffer << std:: endl << std::endl;
		if (_status == INITIAL_STATUS){
			parseRequestLine();
		}
		if (_status == REQUEST_LINE_PARSED){
			parseHeaders();
		}
		if (_status == HEADERS_PARSED){
			parseBody(); //TODO: Timeout
		}
	} catch (const std::exception & e){
		_errorCode = 400;
		std::cerr << e.what() << std::endl;
	}
}

/*----------------- PARSING REQUEST LINE -----------------*/
void	Request::parseRequestLine() {

	size_t posBuffer = _buffer.find("\r\n");
	if (posBuffer == std::string::npos)
		return ;
		//throw std::runtime_error("Error parsing Request: no request line");

	createRequestLineVector(_body.substr(0, posBuffer));
	if (_requestLine.size() != 3)
		throw std::runtime_error("Error parsing Request: wrong request line");

	std::vector<std::string> requestVect = createValidRequestVector(); //TODO: use ServerConfig
	std::vector<std::string>::iterator it = std::find(requestVect.begin(), requestVect.end(), _requestLine.front()); 
	if (it == requestVect.end()) 
		throw std::runtime_error("Error parsing Request: no metod allowed"); //_errorCode = 405;
	
	//TODO: PARSE ROOT _firstLine ??

	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1"))
		throw std::runtime_error("Error parsing Request: bat http version");

	_status = REQUEST_LINE_PARSED;
	_buffer.erase(0, posBuffer + 2); //remove used buffer (request line)
}

void Request::createRequestLineVector(std::string requestLineStr){
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


//----------------- PARSING HEADERS -----------------
/*POST /submit-form HTTP/1.1\r\n
Host: www.example.com\r\n
User-Agent: Mozilla/5.0\r\n
\r\n*/
void	Request::parseHeaders() {
	while (_buffer.find("\r\n") != std::string::npos){
		std::string line = _buffer.substr(0, _buffer.find("\r\n"));
		if (line.size() == 0) { //end of headers >> line = "\r\n" 
			_status = HEADERS_PARSED;
			_buffer.erase(0, 2);
			break;
		}
		else {
			addHeaderToMap(line);
			_buffer.erase(0, line.size() + 2);
		}
	}
	/*for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
}

void	Request::addHeaderToMap(std::string line){
	size_t posColon = line.find(':');
	if (posColon == std::string::npos) 
		throw std::runtime_error("Error parsing Request: no ':' in header");
	std::string name = line.substr(0, posColon);

	std::string value = trim(line.substr(posColon + 1, line.size() - posColon - 1));

	_headers.insert(std::make_pair(name, value));
}


//----------------- PARSING BODY -----------------
void	Request::parseBody(){
	if (_headers.find("Content-Length") == _headers.end() && _headers.find("Transfer-Encoding") == _headers.end()){
		_status = FINISH_PARSED;
	}
	else if (_headers.find("Content-Length") != _headers.end()) {
		parseBodyByContentLenght();
	}
	else if (_headers.find("Transfer-Encoding") != _headers.end()) {
		if (_headers.find("Transfer-Encoding")->second != "chunked")
			throw std::runtime_error("Error parsing Request: wrong Transfer-Encoding parameter");
		parseBodyByChunked();
	}
}

void	Request::parseBodyByContentLenght() {
	if (_buffer.size() > ft_atoi(_headers.find("Content-Length")->second)) {
		throw std::runtime_error("Error parsing Request: Body Lenght greater than Content lenght header param");
	}
	
	size_t posEndBody = _buffer.find("\r\n\r\n");
	if (posEndBody != std::string::npos) //body finished
		_status = FINISH_PARSED;
	_body = _buffer;
	_buffer.clear();
}


/*Chunked Transfer Encoding
7\r\n
Mozilla\r\n
0\r\n
\r\n
*/
void	Request::parseBodyByChunked(){
	
	std::stringstream	ss(_buffer);
	std::string			line;
	int					sizeChunk = -1;				
	
	while (std::getline(ss, line)) {
		/*if (line[line.size() - 1] != '\r')
			throw std::runtime_error("Error Bad Request: Wrong body");
		line.erase(line.size() - 1);
		
		if (sizeChunk < 0) {
			sizeChunk = convertStrToHex(line);
		}
		else {
			if (sizeChunk != line.length())
				throw std::runtime_error("Error Bad Request: Wrong body");
			_body = _body + line;
			sizeChunk = -1;
		} 
		if (sizeChunk == 0) { 
			_status = FINISH_PARSED;
			break ;
		}*/
	}
	//pending remain body
}


bool isStringOfDigits(std::string line){
	for (std::string::iterator it = line.begin(); it != line.end(); it++) {
		 if (!std::isdigit(*it))
			return (false);
	}
	return (true);
}


uint64_t	convertStrToHex(std::string line){
	if (line.empty() || !isStringOfDigits(line))
		throw std::runtime_error("Error Bad Request: Wrong body");
	
	std::stringstream ss;
	ss << std::hex << line;
	uint64_t result;
	ss >> result;

	return result;
}
