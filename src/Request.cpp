#include "Request.hpp"

Request::Request(const std::string & buffer, Socket *socket) : _buffer(buffer), _body(""), _status(INITIAL_STATUS), _socket(socket){
	//TODO: Remain body?? new object?


	//1. estic mirant be el socket? 			OK!
	//2. el socket te el server ben associat?   OK!
	//3. el server te els metodes guardats? 	OK!
	
	
	//Server *serv = _socket->getServer();
	
	//std::cout << "CHECK SEVER ADRESS: " << _socketgetServer() << std::endl;
	std::cout << "CHECK IP: " << _socket->getServer().getIp() << std::endl;
	
	//std::cout << "CHECK PORT of SOCKET: " << _socket->getPort() << std::endl;
	
	//std::cout << "CHECK IP from serv: " << serv->getIp() << std::endl;
	
	//Server *server = _socket->getServer();
	//std::vector<std::string> allowedMethodsVect = server->getAllowedMethods();
	//std::cout << "CHECK IP vol2: " << server->getMaxBodySize() << std::endl;
	//std::vector<std::string> allowedMethodsVect = _socket->getServer()->getAllowedMethods();
	/*for (std::vector<std::string>::iterator ittt = allowedMethodsVect.begin(); ittt != allowedMethodsVect.end(); ++ittt) {
		std::cout << "Method allowed" << *ittt << std::endl;
	}*/
}

Request::~Request() {
}

/*	REQUEST LINE: method | URI and protocol | version
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/


/*----------------- PARSING REQUEST LINE -----------------*/

void	Request::parseRequest() {
	try {
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
	std::cout << "REQUEST: " << std::endl << _buffer << std::endl << "----end Request---" << std::endl;

	size_t posBuffer = _buffer.find("\r\n");
	if (posBuffer == std::string::npos){
		std::cout << "not complete request line: " << std::endl;
		return ;
	}
	createRequestLineVector(_buffer.substr(0, posBuffer));
	if (_requestLine.size() != 3)
		throw std::runtime_error("Error parsing Request: wrong request line");
	
	std::cout << "Paass[49]"<< std::endl;
	
	std::vector<std::string> allowedMethodsVect = _socket->getServer().getAllowedMethods();

	std::cout << "Paass[53]"<< std::endl;

	for (std::vector<std::string>::iterator ittt = allowedMethodsVect.begin(); ittt != allowedMethodsVect.end(); ++ittt) {
		std::cout << "Method allowed" << *ittt << std::endl;
	}
	std::vector<std::string>::iterator it = std::find(allowedMethodsVect.begin(), allowedMethodsVect.end(), _requestLine.front()); 
	
	if (it == allowedMethodsVect.end())
		throw std::runtime_error("Error parsing Request: no metod allowed"); //_errorCode = 405;
	
	//TODO: PARSE ROOT _firstLine ??

	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1"))
		throw std::runtime_error("Error parsing Request: bat http version");

	_status = REQUEST_LINE_PARSED;
	_buffer.erase(0, posBuffer + 2); //remove used buffer (request line)
}

void Request::createRequestLineVector(std::string requestLineStr){
	std::cout << "REQUEST LINE: " << requestLineStr << std::endl;
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
		std::cout << "PARAMS REQ LINE " << element << std::endl;
	}
	std::cout << "----END request line -----"<< std::endl;
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

	if (_headers.find("Transfer-Encoding") != _headers.end()) {
		if (_headers.find("Transfer-Encoding")->second != "chunked")
			throw std::runtime_error("Error parsing Request: wrong Transfer-Encoding value parameter");
		parseBodyByChunked();
	}
	else if (_headers.find("Content-Length") != _headers.end()) {
		parseBodyByContentLength();
	}
	else {
		_status = FINISH_PARSED;
	}
}

void	Request::parseBodyByContentLength() { 
	long unsigned int contentLength = ft_atoi(_headers.find("Content-Length")->second);
	
	if (_buffer.size() > contentLength ) {  //TODO: manage remain body?? Adria - create new request with remain body
		throw std::runtime_error("Error parsing Request: Body Length greater than Content length header param");
	}
	if (contentLength > 2000) { //TODO: update with server server.getMaxBodySize()
		throw std::runtime_error("Error parsing Request: Body Length greater than Max body size");
	}
	while (_body.length() < contentLength && _buffer.length() > 0) {
		_body.push_back(_buffer[0]);
		_buffer.erase(0);
	}
	if (_body.length() == contentLength) {
		_status = FINISH_PARSED;
	}
}


/*Chunked Transfer Encoding
7\r\n
Mozilla\r\n
0\r\n
\r\n
*/
//https://datatracker.ietf.org/doc/rfc9112/ - 7.1.3.  Decoding Chunked
void	Request::parseBodyByChunked(){
	std::string			line;
	int					sizeChunk = 0;
	//int					length;
	size_t				posEndSIze;

	posEndSIze = _buffer.find("\r\n");
	if (posEndSIze == std::string::npos){
		return;
	}

	line = line.substr(0, posEndSIze);
	sizeChunk = convertStrToHex(line);

	while (sizeChunk > 0) {
		if (sizeChunk + 2 - posEndSIze - 2 > _buffer.length())
			return;
		if (_buffer.find("\r\n", posEndSIze + 2 + sizeChunk) == std::string::npos)
			throw std::runtime_error("Error parsing Request: Transfre encoding it is not okay");
		if (_buffer.substr(posEndSIze + 2, sizeChunk).length() + _body.length() > 2000) //TODO: update with server server.getMaxBodySize()
			throw std::runtime_error("Error parsing Request: Body is too large");
		
		_body = _body + _buffer.substr(posEndSIze + 2, sizeChunk);
		_buffer.erase(posEndSIze + sizeChunk + 4);
	}

	if (sizeChunk == 0) {
		_status = FINISH_PARSED;
		_headers.find("Transfer-Encoding")->second = ""; //remove chunked
		size_t length = _body.length();
		_headers.insert(std::make_pair("Content-Length", ft_itoa(length)));
	}
}

bool Request::isStringOfDigits(std::string line){
	for (std::string::iterator it = line.begin(); it != line.end(); it++) {
		 if (!std::isdigit(*it))
			return (false);
	}
	return (true);
}

uint64_t	Request::convertStrToHex(std::string line){
	
	size_t endSizeChunk = line.find(' ');

	if (endSizeChunk != std::string::npos)
		line = line.substr(0, endSizeChunk);

	if (line.empty() || !isStringOfDigits(line))
		throw std::runtime_error("Error Bad Request: Wrong body");
	
	std::stringstream ss;
	ss << std::hex << line;
	uint64_t result;
	ss >> result;

	return result;
}


