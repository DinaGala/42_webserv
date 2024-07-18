#include "Request.hpp"

Request::Request(Socket& socket) : _status(INITIAL_STATUS), _socket(socket){
	initParamsRequest();
}

Request::~Request() {
}

void	Request::initParamsRequest() {
	_buffer.clear();
	_requestLine.clear();
	_headers.clear();
	_body = "";
	_query = "";
	_path = "";
	_code = 200;
	_maxBodySize = _socket.getServer().getMaxBodySize();
	_allowedMethods = _socket.getServer().getAllowedMethods();
	_errorPages = _socket.getServer().getErrorPages();
	_index =  "";
	_autoIndex = _socket.getServer().getAutoIndex();
	_allowUpload = false;
	_uploadDir = "";
	_return = "";
	_cgi = false;
	_cgiConf = _socket.getServer().getCgiConf();
	_serverNames = _socket.getServer().getServerNames();
	_connectionKeepAlive = true;
	_boundary = "";
	_fileName = "";
}

//TODO: Accepted content: Accept: text/html, application/xhtml+xml, application/xml;q=0.9, image/webp, */*;q=0.8

/*	REQUEST LINE: method | URI | and protocolversion
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/

/*----------------- PARSING REQUEST LINE -----------------*/
void	Request::parseRequest(const std::string& buffer) {
	_buffer = _buffer + buffer;
	std::cout << "REQUEST -------------" << std::endl;
	std::cout << _buffer << std::endl;
	
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
		if (_code == 200)
			_code = 400;
		std::cerr << e.what() << std::endl;
		_status = FINISH_PARSED;
		
	}
}

/*----------------- PARSING REQUEST LINE -----------------*/
void	Request::parseRequestLine() {
	size_t posBuffer = _buffer.find("\r\n");
	if (posBuffer == std::string::npos)
		return ;
	createRequestLineVector(_buffer.substr(0, posBuffer));

	checkPath();
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

void Request::checkPath(){
/*std::string	Response::_parseUrl(const std::string &url)
{
	std::string::size_type	found = url.find(this->_host);
	std::string::size_type	next;
	std::string str;

	if (found != std::string::npos)
		found += this->_host.size() + 1;
	else
		found = 0;
	next = url.find("?", found);
	if (next != std::string::npos)
	{
		this->_reqbody = url.substr(next, url.size());
		str = url.substr(found, next);
	}
	else
		str = url.substr(found);
	return (str);
}*/
}

void Request::checkAllowMethod(){
	std::vector<std::string> allowedMethodsVect = _socket.getServer().getAllowedMethods();
	std::vector<std::string>::iterator it = std::find(allowedMethodsVect.begin(), allowedMethodsVect.end(), _requestLine.front()); 
	if (it == allowedMethodsVect.end()) {
		_code = 405;
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
			addHeaderToMap(line, _headers);
			_buffer.erase(0, line.size() + 2);
		}
	}
	/*for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
	if (_status == HEADERS_PARSED){
		checkConnectionKeepAlive();
		checkAcceptedContent();
	}
}

void	Request::checkConnectionKeepAlive() {
	if (_headers.find("Connection") != _headers.end() && _headers.find("Connection")->second == "close") {
		_connectionKeepAlive = false;
	}
}

void	Request::checkAcceptedContent() {
	if (_headers.find("Accept") != _headers.end()) {
		std::vector<std::string> acceptVec = ft_split(_headers.find("Accept")->second, ",");
		for (unsigned int i=0; i < acceptVec.size(); i++) {
			size_t posSlash = acceptVec[i].find('/');
			if (posSlash == std::string::npos)
				throw std::runtime_error("Error parsing Request: bas headers");
			std::string type = trim(acceptVec[i].substr(0, posSlash));
			std::string subtype = trim(acceptVec[i].substr(posSlash + 1, acceptVec[i].length() - posSlash -1));
			
			size_t posSemicolon = subtype.find(';');
			if (posSemicolon != std::string::npos) {
				subtype = subtype.substr(0, posSemicolon);
			}
			_acceptedContent.insert(std::make_pair(type, subtype));
		}
	}
}

void	Request::addHeaderToMap(std::string& line, std::map<std::string, std::string>& map){
	size_t posColon = line.find(':');
	if (posColon == std::string::npos) 
		throw std::runtime_error("Error parsing Request: bas headers");
	
	std::string name = trim(line.substr(0, posColon));
	std::string value = trim(line.substr(posColon + 1, line.size() - posColon - 1));

	map.insert(std::make_pair(name, value));
}


//----------------- PARSING BODY -----------------
void	Request::parseBody(){
	if (_headers.find("Transfer-Encoding") == _headers.end() && _headers.find("Content-Length") == _headers.end()) {
		_status = FINISH_PARSED;
		return ;
	}	
	if (_headers.find("Transfer-Encoding") != _headers.end()) {
		if (_headers.find("Transfer-Encoding")->second.empty() || _headers.find("Transfer-Encoding")->second != "chunked")
			throw std::runtime_error("Error parsing Request: wrong Transfer-Encoding value parameter");
		parseBodyByChunked();
	}
	else if (_headers.find("Content-Length") != _headers.end())
		parseBodyByContentLength();
	if (_status == BODY_PARSED)
		manageMultipartForm();
	_status = FINISH_PARSED;
}

void	Request::manageMultipartForm(){
	if (_headers.find("Content-Type") != _headers.end() && !_headers.find("Content-Type")->second.empty() && _headers.find("Content-Type")->second.find("multipart/form-data")) {
		getBoundary();
		saveMultipartHeaders();
		updateMultipartBody();
		saveFileName();
	}
} 

void	Request::getBoundary() {
	std::string content = _headers.find("Content-Type")->second;
	size_t pos = content.find("boundary=");
	if (pos == std::string::npos) 
		throw std::runtime_error("Error parsing Request: wrong Content-Type parameter");
	_boundary = content.substr(pos+9, content.length() - (pos+9));
}

void	Request::saveMultipartHeaders() {
	size_t pos = _body.find("--" + _boundary);
	if (pos == std::string::npos)
		throw std::runtime_error("Error parsing Request: wrong Content-Type parameter");
	
	size_t startPos = _boundary.length() + pos + 4; //4 - \r\n + initial --
	while (_body.find("\r\n", startPos) != std::string::npos){
		std::string line = _body.substr(startPos, _body.find("\r\n", startPos) - startPos);
		if (line.size() == 0)  
			break;
		else 
			addHeaderToMap(line, _multipartHeaders);
		startPos = _body.find("\r\n", startPos) + 2; 
	}
		
	if (_multipartHeaders.find("Content-Type") == _multipartHeaders.end() || _multipartHeaders.find("Content-Disposition") == _multipartHeaders.end())
		throw std::runtime_error("Error parsing Request: wrong Content-Type parameter");
	/*for (std::map<std::string, std::string>::iterator it = _multipartHeaders.begin(); it != _multipartHeaders.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
}

void	Request::updateMultipartBody() {
	size_t startBody = _body.find("\r\n\r\n") + 4;
	if (startBody == std::string::npos)
		throw std::runtime_error("Error parsing Request: wrong Content-Type parameter");

	size_t finishBody = _body.find(_boundary + "--", startBody);
	
	if (finishBody == std::string::npos)
		throw std::runtime_error("Error parsing Request: wrong Content-Type parameter");
	_body.erase(finishBody);
	_body.erase(0, startBody);
}

//Content-Disposition: form-data; name="file"; filename="example.txt"
void	Request::saveFileName() {
	std::string content = _multipartHeaders.find("Content-Disposition")->second;
	size_t auxPos = content.find("filename");
	if (auxPos == std::string::npos)
		return;
	size_t initFilename = content.find('\"', auxPos);
	size_t endFilename = content.find('\"', initFilename + 1);
	_fileName = content.substr(initFilename + 1, endFilename - initFilename);
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
	if (_body.length() == contentLength)
		_status = BODY_PARSED;
	
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
		_status = BODY_PARSED;
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

const Socket&  Request::getSocket() const {
	return (_socket);
}

const std::map<std::string, std::string>& Request::getHeaders() const{
	return (_headers);
}

const std::string& Request::getBody() const {
	return (_body);
}

const std::string& Request::getQuery() const {
	return (_query);
}

const std::string&  Request::getPath() const {
    return (_requestLine[1]);
}

int	Request::getCode() const {
	return (_code);
}

size_t	Request::getMaxBodySize() const {
	return (_maxBodySize);
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

const std::string& 	Request::getUploadDir() const {
	return (_uploadDir);
}

const std::string& 	Request::getReturn() const {
	return (_return);
}

bool Request::getCgi() const {
	return (_cgi);
}

const std::map<std::string, std::string>&  Request::getCgiConf() const {
	return (_cgiConf);
}

const std::string&	Request::getMethod() const {
	return (_requestLine[0]);
}

const std::vector<std::string>& Request::getServerNames() const{
	return (_serverNames);
}

bool Request::getConnectionKeepAlive() const {
	return (_connectionKeepAlive);
}

const std::multimap<std::string, std::string>&	Request::getAcceptedContent() const {
	return (_acceptedContent);
}

const std::map<std::string, std::string>& Request::getMultipartHeaders() const{
	return (_multipartHeaders);
}

const std::string&	Request::getFileName() const {
	return (_fileName);
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
