#include "Request.hpp"

// Request::Request(const std::string& buffer, Socket& socket) : _buffer(buffer), _status(INITIAL_STATUS), _socket(socket){
// 	initParamsRequest();
// 	std::cout << "REQUEST -------------" << std::endl;
// 	std::cout << _buffer << std::endl;
// }

Request::Request(Server& serv): _serv(serv)
{
	initParamsRequest();
	std::cout << "REQUEST -------------" << std::endl;
//	std::cout << _buffer << std::endl;
}

Request::~Request() {
}

Request& Request::operator=(const Request& src)
{
	_serv = src._serv;
//	FINISH
//	empty = src.empty;
	return (*this);
}

Request::Request(const Request& src): _serv(src._serv)
{
	*this = src;
}

void	Request::initParamsRequest() {
	
	_buffer.clear();
	_requestLine.clear();
	_headers.clear();
	_body = "";
	_errorCode = 200; //TODO: error code default?
	_maxBodySize = _serv.getMaxBodySize();
	_allowedMethods = _serv.getAllowedMethods();
	_errorPages = _serv.getErrorPages();
	_index =  "";
	_autoIndex = _serv.getAutoIndex();
	_allowUpload = false;
	_uploadDir = "";
	_return = "";
	_cgiConf = _serv.getCgiConf();
}

void	Request::cleanRequest() {
	
	_buffer.clear();
	_requestLine.clear();
	_headers.clear();
	_body = "";
	_errorCode = 200; //TODO: error code default?
	_status = 0;
	_connectionKeepAlive = true;
}

/*	REQUEST LINE: method | URI and protocol | version
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/

/*----------------- PARSING REQUEST LINE -----------------*/
void	Request::parseRequest(std::string buffer) {
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
		if (_errorCode == 200)
			_errorCode = 400;
		std::cerr << e.what() << std::endl;
	}
}

/*----------------- PARSING REQUEST LINE -----------------*/
void	Request::parseRequestLine() {
	size_t posBuffer = _buffer.find("\r\n");
	if (posBuffer == std::string::npos)
		return ;
	createRequestLineVector(_buffer.substr(0, posBuffer));

	checkUrlPath();
	checkProtocolHttp();
	checkAllowMethod(); //TOD: Pending update methods allowed of location

	_status = REQUEST_LINE_PARSED;
	_buffer.erase(0, posBuffer + 2); //remove used buffer (request line)
}

void Request::createRequestLineVector(std::string requestLineStr){
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
	}
	if (_requestLine.size() != 3)
		throw std::runtime_error("Error parsing Request: wrong request line");
}

void Request::checkUrlPath(){

}

void Request::checkAllowMethod(){
	std::vector<std::string> allowedMethodsVect = _serv.getAllowedMethods();
	std::vector<std::string>::iterator it = std::find(allowedMethodsVect.begin(), allowedMethodsVect.end(), _requestLine.front()); 
	if (it == allowedMethodsVect.end()) {
		_errorCode = 405;
		throw std::runtime_error("Error parsing Request: no metod allowed");
	}
}

void Request::checkProtocolHttp(){
	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1"))
		throw std::runtime_error("Error parsing Request: bat http version");
}

/*----------------- PARSING HEADERS -----------------*/

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
		throw std::runtime_error("Error parsing Request: bas headers");
	
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
	if (contentLength > _maxBodySize) {
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
		if (_buffer.substr(posEndSIze + 2, sizeChunk).length() + _body.length() > _maxBodySize)
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

// _____________  GETTERS _____________ 

// const Socket&  Request::getSocket() const {
// 	return (_socket);
// }

const Server&  Request::getServ() const {
	return (_serv);
}

const std::vector<std::string>& Request::getAllowedMethods() const{
	return (_allowedMethods);
}

const std::map<int, std::string>& Request::getErrorPages() const {
	return (_errorPages);
}

const std::string& Request::getIndex() const {
	return (_index);
}

bool Request::getAutoIndex() const {
	return (_autoIndex);
}

bool Request::getAllowUpload() const {
	return (_allowUpload);
}

const int Request::getStatus() const {
	return (_status);
}

const std::string& 	Request::getUploadDir() const {
	return (_uploadDir);
}

const std::string& 	Request::getReturn() const {
	return (_return);
}

const std::map<std::string, std::string>&  Request::getCgiConf() const {
	return (_cgiConf);
}

bool	Request::getConnectionKeepAlive() const
{
	return (_connectionKeepAlive);
}

// _____________  SETTERS _____________ 

void Request::setErrorPages(const std::map<int, std::string>&  errorPages) {
	_errorPages = errorPages;
}

void Request::setIndex(const std::string& index) {
	_index = index;
}

void Request::setAutoIndex(bool autoindex) {
	_autoIndex = autoindex;
}

void Request::setAllowUpload(bool allowUpload) {
	_allowUpload = allowUpload;
}

void Request::setUploadDir(const std::string& uploadDir) {
	_uploadDir = uploadDir;
}

void Request::setReturn(const std::string& alias) {
	_return = alias;
}

void Request::setCgiConf(const std::map<std::string, std::string>& cgiConf) {
	_cgiConf = cgiConf;
}
