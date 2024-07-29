#include "Request.hpp"

Request::Request(Server& server, int port) : _server(server), _port(port)
{
	initParams();
}

Request::~Request() 
{
}

Request& Request::operator=(const Request& src)
{
	_buffer = src.getBuffer();
	_status = src.getStatus();
	_server = src.getServer();
	_port = src.getPort();
	_requestLine = src.getRequesLine();
	_headers = src.getHeaders();
	_body = src.getBody();
	_query = src.getQuery();
	_path = src.getPath();
	_root = src.getRoot();
	_method = src.getMethod();
	_code = src.getCode();
	_host = src.getHost();
	_maxBodySize = src.getMaxBodySize();
	_allowedMethods = src.getAllowedMethods();
	_errorPages = src.getErrorPages();
	_index = src.getIndex();
	_autoIndex = src.getAutoIndex();
	_allowUpload = getAllowUpload();
	_uploadDir = src.getUploadDir();
	_return = src.getReturn(); 
	_cgi = src.getCgi();
	_cgiConf = src.getCgiConf();
	_serverNames = src.getServerNames(); 
	_connectionKeepAlive = src.getConnectionKeepAlive();
	_acceptedContent = src.getAcceptedContent();
	_boundary = src.getBoundary();
	_multipartHeaders = src.getMultipartHeaders();
	_fileName = src.getFileName();
	_location = src.getLocation();
	return (*this);
}

Request::Request(const Request& src): _server(src._server)
{
	*this = src;
}

void	Request::initParams() 
{
	_buffer.clear();
	_status = INITIAL_STATUS;
	_requestLine.clear();
	_headers.clear();
	_body = "";
	_query = "";
	_path = "";
	_root = "";
	_method = "";
	_code = 200;
	_maxBodySize = 0;
	_host = "";
	_allowedMethods.clear();
	_errorPages.clear();
	_index =  ""; //TODO: set as default
	_autoIndex = false;
	_allowUpload = false;
	_uploadDir = "";
	_return = "";
	_cgi = false;
	_cgiConf.clear();
	_serverNames.clear();
	_connectionKeepAlive = true;
	_multipartHeaders.clear();
	_acceptedContent.clear();
	_boundary = "";
	_fileName = "";
	_location = false;
}

void	Request::setServerParams() 
{
	_host = _server.getHost();
	_maxBodySize = _server.getMaxBodySize();
	_allowedMethods = _server.getAllowedMethods();
	_errorPages = _server.getErrorPages();
	_autoIndex = _server.getAutoIndex();
	_return = _server.getReturn();
	_cgiConf = _server.getCgiConf();
	_serverNames = _server.getServerName();
}

/*	REQUEST LINE: method | URI | and protocolversion
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/

// _____________  PARSING REQUEST  _____________ 
void	Request::parseRequest(const std::string& buffer) 
{

	_buffer = _buffer + buffer;
	std::cout << "REQUEST -------------" << std::endl;
	std::cout << _buffer << std::endl;
	setServerParams();
	try {
		if (_status == INITIAL_STATUS){
			parseRequestLine();
		}
		if (_status == REQUEST_LINE_PARSED){
			parseHeaders();
		}
		if (_status == HEADERS_PARSED){
			parseBody();
		}
		if (_status == FINISH_PARSED){
			std::cout << "FINISH REQUEST PARSING" << std::endl;
			requestValidations();
		}
	} catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
		_status = FINISH_PARSED;
	}
}

// _____________  PARSING REQUEST LINE  _____________ 
void	Request::parseRequestLine() {
	size_t posBuffer = _buffer.find(CRLF);
	if (posBuffer == std::string::npos)
		return ;
	createRequestLineVector(_buffer.substr(0, posBuffer));

	_status = REQUEST_LINE_PARSED;
	_buffer.erase(0, posBuffer + 2);

	/*std::cout << "REQ LINE -------------" << std::endl;
	for (std::vector<std::string>::iterator it = _requestLine.begin(); it != _requestLine.end(); ++it) {
		std::cout << *it << std::endl;
	}*/
}

void Request::createRequestLineVector(std::string requestLineStr){
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
	}
	if (_requestLine.size() != 3) 
		sendBadRequestError("Request parsing error: invalid request line");
	_method = _requestLine[0]; 
}

// _____________  PARSING HEADERS   _____________ 
void	Request::parseHeaders() {
	while (_buffer.find(CRLF) != std::string::npos){
		std::string line = _buffer.substr(0, _buffer.find(CRLF));
		if (line.size() == 0) { //end of headers >> line = CRLF 
			_status = HEADERS_PARSED;
			_buffer.erase(0, 2);
			break;
		}
		else {
			addHeaderToMap(line, _headers);
			_buffer.erase(0, line.size() + 2);
		}
	}
	/*std::cout << "HEADERS -------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
}

void	Request::addHeaderToMap(std::string& line, std::map<std::string, std::string>& map){
	size_t posColon = line.find(':');
	if (posColon == std::string::npos) 
		sendBadRequestError("Request parsing error: invalid headers");
	std::string name = trim(line.substr(0, posColon));
	std::string value = trim(line.substr(posColon + 1, line.size() - posColon - 1));

	map.insert(std::make_pair(name, value));
}

// _____________  PARSING BODY  _____________ 
void	Request::parseBody(){
	if (_headers.find("Transfer-Encoding") == _headers.end() && _headers.find("Content-Length") == _headers.end()) {
		_status = FINISH_PARSED;
		return ;
	}
	if (_headers.find("Transfer-Encoding") != _headers.end()) {
		if (_headers.find("Transfer-Encoding")->second != "chunked") 
			sendBadRequestError("Request parsing error: invalid Transfer-Encoding value parameter");
		parseBodyByChunked();
	}
	else if (_headers.find("Content-Length") != _headers.end())
		parseBodyByContentLength();
	if (_status == BODY_PARSED) {
		manageMultipartForm();
		_status = FINISH_PARSED;
	}
}

//Chunked Transfer Encoding
//https://datatracker.ietf.org/doc/rfc9112/ - 7.1.3.  Decoding Chunked
void	Request::parseBodyByChunked(){
	int	sizeChunk = 0;
	do {
		size_t	posEndSIze = _buffer.find(CRLF);
		sizeChunk = findSizeChunk(posEndSIze);
		if (sizeChunk <= 0 || posEndSIze + sizeChunk + 4 > _buffer.length()) //final of shunk or chunk incompleted
			break ;
		size_t posEndLine = _buffer.find(CRLF, posEndSIze + 2 + sizeChunk);
		if (posEndLine == std::string::npos || posEndLine != posEndSIze + 2 + sizeChunk) //there is not CRLF at the end of chunk
			sendBadRequestError("Request parsing error: invalid Transfre encoding");
		manageLineChunk(posEndSIze, sizeChunk);
	} while (1);

	if (sizeChunk == 0) {
		_status = BODY_PARSED;
		_headers.find("Transfer-Encoding")->second = "";
		_headers.insert(std::make_pair("Content-Length", ft_itoa(_body.length())));
		_buffer.clear();
	}
}

int	Request::findSizeChunk(size_t posEndSIze) {
	if (posEndSIze == std::string::npos) //chunk is not completed
		return (-1);
	std::string line = _buffer.substr(0, posEndSIze);
	return (convertStrToHex(line));
}

uint64_t	Request::convertStrToHex(std::string line){
	size_t endSizeChunk = line.find(' ');
	if (endSizeChunk != std::string::npos)
		line = line.substr(0, endSizeChunk);
	if (line.empty() || !isStringOfDigits(line)) 
		sendBadRequestError("Error Bad Request: Invalid shunked body");
	uint64_t result = strToHex(line);
	return (result);
}

void	Request::manageLineChunk(size_t posEndSIze, int sizeChunk) {
	std::string line = _buffer.substr(posEndSIze + 2, sizeChunk);
		
	if (line.length() + _body.length() > _maxBodySize)
		sendBadRequestError("Request parsing error: Body is too large");
	_body = _body + line;
	_buffer.erase(0, posEndSIze + sizeChunk + 4);
}

void	Request::parseBodyByContentLength() { 
	long unsigned int contentLength = ft_atoi(_headers.find("Content-Length")->second);
	if (contentLength > _maxBodySize)
		sendBadRequestError("Request parsing error: Body Length greater than Max body size");
	while (_body.length() < contentLength && _buffer.length() > 0) {
		_body.push_back(_buffer[0]);
		_buffer.erase(0);
	}
	if (_body.length() == contentLength)
		_status = BODY_PARSED;
}

// _____________  MULTIPART FORM BODY   _____________ 
void	Request::manageMultipartForm(){
	if (_headers.find("Content-Type") != _headers.end() && _headers.find("Content-Type")->second.find("multipart/form-data")) {
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
		sendBadRequestError("Request parsing error: invalid Content-Type parameter");
	_boundary = content.substr(pos+9, content.length() - (pos+9));
}

void	Request::saveMultipartHeaders() {
	size_t pos = _body.find("--" + _boundary);
	if (pos == std::string::npos)
		sendBadRequestError("Request parsing error: invalid Content-Type parameter");
	size_t startPos = _boundary.length() + pos + 4; //4 - \r\n + initial --
	while (_body.find(CRLF, startPos) != std::string::npos){
		std::string line = _body.substr(startPos, _body.find(CRLF, startPos) - startPos);
		if (line.size() == 0)  
			break;
		else 
			addHeaderToMap(line, _multipartHeaders);
		startPos = _body.find(CRLF, startPos) + 2; 
	}
		
	if (_multipartHeaders.find("Content-Type") == _multipartHeaders.end() || _multipartHeaders.find("Content-Disposition") == _multipartHeaders.end())
		sendBadRequestError("Request parsing error: invalid Multipart headers");
	/*for (std::map<std::string, std::string>::iterator it = _multipartHeaders.begin(); it != _multipartHeaders.end(); ++it) {
		std::cout << it->first << " :: " << it->second << std::endl;
	}*/
}

void	Request::updateMultipartBody() {
	size_t startBody = _body.find("\r\n\r\n") + 4;
	if (startBody == std::string::npos)
		sendBadRequestError("Request parsing error: invalid Content-Type parameter");
	size_t finishBody = _body.find(_boundary + "--", startBody);
	
	if (finishBody == std::string::npos) 
		sendBadRequestError("Request parsing error: invalid Content-Type parameter");
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

// _____________  SEND BAD REQUEST ERROR  _____________ 
void	Request::sendBadRequestError(std::string errMssg) {
	throw std::runtime_error(errMssg);
	_code = 400;
}

// _____________  VALIDTATE REQUEST  _____________ 
void Request::requestValidations(){
	checkHost(); 	
	checkConnectionKeepAlive();
	manageAcceptedContent();
	managePath();
	checkProtocolHttp();
	checkAllowMethod();
}
 
void	Request::checkConnectionKeepAlive() {
	if (_headers.find("Connection") != _headers.end() && _headers.find("Connection")->second == "close") {
		_connectionKeepAlive = false;
	}
}

void	Request::checkHost() {
	if (_headers.find("Host") == _headers.end())
		sendBadRequestError("Request parsing error: invalid Host");
	//TODO: CHeck host + port
	std::string hostHeader = _headers.find("Host")->second;
	std::string hostSaved1 = _server.getIpAdress() + ":" + ft_itoa(_port);
	std::string hostSaved2 = _server.getHost() + ":" + ft_itoa(_port);
	if (hostHeader ==  hostSaved1 || hostHeader ==  hostSaved2)
		return;
	//TODO: Manage server names
}

void	Request::manageAcceptedContent() {
	if (_headers.find("Accept") == _headers.end())
		return ;
	std::vector<std::string> acceptVec = ft_split(_headers.find("Accept")->second, ",");
	for (unsigned int i=0; i < acceptVec.size(); i++) {
		size_t posSlash = acceptVec[i].find('/');
		if (posSlash == std::string::npos) 
			sendBadRequestError("Request parsing error: invalid Accept header");
		std::string type = trim(acceptVec[i].substr(0, posSlash));
		std::string subtype = trim(acceptVec[i].substr(posSlash + 1, acceptVec[i].length() - posSlash -1));
		
		size_t posSemicolon = subtype.find(';');
		if (posSemicolon != std::string::npos) {
			subtype = subtype.substr(0, posSemicolon);
		}
		_acceptedContent.insert(std::make_pair(type, subtype));
	}
}

void Request::managePath() {
	checkQuery();
	size_t nLoc = checkLocation(); //TODO: PENDING TO CHECK
	if (_location)
		updateInfoLocation(nLoc);
	updatePath();  //TODO: PENDING TO CHECK
}

void	Request::checkQuery() {
	std::string url = _requestLine[1];
	std::string::size_type	posQuery;

	posQuery = url.find("?");
	if (posQuery != std::string::npos) {
		_query = url.substr(posQuery + 1, url.size() - posQuery);
		_path = url.substr(0, posQuery);
	}
	else {
		_path = url;
	}
}

size_t Request::checkLocation() {
	std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
	size_t 	posLoc = 0;
	size_t	nEqualLocs = 0;
	size_t 	j=0;

	std::vector<std::string> vecPath = ft_split(_path, "/");
	
	std::cout << "VEC PATH -------------" << std::endl;
	for (std::vector<std::string>::iterator it = vecPath.begin(); it != vecPath.end(); ++it) {
		std::cout << *it << std::endl;
	}

	for (size_t i=0; i < vecLocations.size(); i++) {
		std::vector<std::string> vecLoc = ft_split(vecLocations[i].getUri(), "/");
		std::cout << "VEC LOC " << i << " ----" << std::endl;
		for (std::vector<std::string>::iterator itt = vecLoc.begin(); itt != vecLoc.end(); ++itt) {
			std::cout << *itt << std::endl;
		}
		for (j = 0; j < vecLoc.size(); j++) {
			std::cout << "COMPARE " << vecPath[j] << " vs " << vecLoc[j] << std::endl;
			if (vecPath[j] != vecLoc[j]) {
				break;
			}	
		}
		if (j > nEqualLocs) {
			posLoc = i;
			nEqualLocs = j;
			_location = true;
		}
	}
	std::cout << "POSITION LOC " << posLoc << std::endl;
	return posLoc;
}

void Request::updatePath() {
	std::string str = _requestLine[1];
	if (_requestLine[0] == "GET" || _requestLine[0] == "DELETE"){
		_path = str.insert(0, ".");
		if (access(str.c_str(), X_OK) == 0)
			this->_cgi = true;
	} 
	else if (_requestLine[0] == "POST" && _return != ""){
		_path = _return;
		_code = 301;
	}
}

void	Request::updateInfoLocation(size_t nLoc) {
 	std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
    LocationConfig location = vecLocations[nLoc];

	_root = vecLocations[nLoc].getRoot();
	_allowedMethods = vecLocations[nLoc].getAllowedMethods();
	_errorPages = vecLocations[nLoc].getErrorPages();
	_index =  vecLocations[nLoc].getIndex();
	_autoIndex = vecLocations[nLoc].getAutoIndex();
	_allowUpload = vecLocations[nLoc].getAllowUpload();
	_uploadDir = vecLocations[nLoc].getUploadDir();
	_return = vecLocations[nLoc].getReturn();
	_cgiConf = vecLocations[nLoc].getCgiConf();
}

void Request::checkAllowMethod(){
	std::vector<std::string> allowedMethodsVect = _server.getAllowedMethods();
	std::vector<std::string>::iterator it = std::find(allowedMethodsVect.begin(), allowedMethodsVect.end(), _requestLine.front()); 
	if (it == allowedMethodsVect.end()) {
		_code = 405;
		throw std::runtime_error("Request parsing error: method not allowed");
	}
}

void Request::checkProtocolHttp(){
	if (_requestLine.size() == 3) {
		if (strcmp(_requestLine[2].c_str(), "HTTP/1.1")) 
		sendBadRequestError("Request parsing error: invalid http version");
	}
	else
		sendBadRequestError("Request parsing error: invalid request line");
}

// _____________  GETTERS _____________ 

const std::string& Request::getBuffer() const 
{
	return (_buffer);
}

Server&  Request::getServer() const 
{
	return (_server);
}

int  Request::getPort() const 
{
	return (_port);
}

int	Request::getStatus() const {
	return (_status);
}

const std::vector<std::string>& Request::getRequesLine() const
{
	return (_requestLine);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
	return (_headers);
}

const std::string& Request::getBody() const 
{
	return (_body);
}

const std::string& Request::getQuery() const 
{
	return (_query);
}

const std::string&  Request::getPath() const 
{
    return (_path);
}

const std::string& 	Request::getRoot() const
{
    return (_root);
}

const std::string&	Request::getMethod() const 
{
	return (_method);
}

int	Request::getCode() const 
{
	return (_code);
}

size_t	Request::getMaxBodySize() const 
{
	return (_maxBodySize);
}

const std::string& Request::getHost() const 
{
	return (_host);
}

const std::vector<std::string>& Request::getAllowedMethods() const
{
	return (_allowedMethods);
}

const std::map<int, std::pair<std::string, std::string> >& Request::getErrorPages() const 
{
	return (_errorPages);
}

const std::string& Request::getIndex() const 
{
	return (_index);
}

bool Request::getAutoIndex() const 
{
	return (_autoIndex);
}

bool Request::getAllowUpload() const 
{
	return (_allowUpload);
}

const std::string& 	Request::getUploadDir() const 
{
	return (_uploadDir);
}

const std::string& 	Request::getReturn() const 
{
	return (_return);
}

bool Request::getCgi() const
{
	return (_cgi);
}

const std::map<std::string, std::string>&  Request::getCgiConf() const 
{
	return (_cgiConf);
}

const std::vector<std::string>& Request::getServerNames() const
{
	return (_serverNames);
}

bool Request::getConnectionKeepAlive() const 
{
	return (_connectionKeepAlive);
}

const std::multimap<std::string, std::string>&	Request::getAcceptedContent() const 
{
	return (_acceptedContent);
}

const std::string&  Request::getBoundary() const 
{
	return (_boundary);
}

const std::map<std::string, std::string>& Request::getMultipartHeaders() const
{
	return (_multipartHeaders);
}

const std::string&	Request::getFileName() const 
{
	return (_fileName);
}

bool Request::getLocation() const 
{
	return (_location);
}

// _____________  SETTERS _____________ 

/*void Request::setErrorPages(const std::map<int, std::pair<std::string, std::string> >&  errorPages) 
{
	_errorPages = errorPages;
}*/

void Request::setIndex(const std::string& index) 
{
	_index = index;
}

void Request::setAutoIndex(bool autoindex) 
{
	_autoIndex = autoindex;
}

void Request::setAllowUpload(bool allowUpload) 
{
	_allowUpload = allowUpload;
}

void Request::setUploadDir(const std::string& uploadDir) 
{
	_uploadDir = uploadDir;
}

void Request::setReturn(const std::string& alias) 
{
	_return = alias;
}

void Request::setCgiConf(const std::map<std::string, std::string>& cgiConf) 
{
	_cgiConf = cgiConf;
}
