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
	_requestLine = src.getRequestLine();
	_headers = src.getHeaders();
	_body = src.getBody();
	_query = src.getQuery();
	_path = src.getPath();
	_root = src.getRoot();
	_rootLoc = src.getRootLoc(); 
	_method = src.getMethod();
	_code = src.getCode();
	_posLocation = src.getPosLocation();
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
	_cookiesEnv = src.getCookiesEnv();
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
	_root = _server.getRoot();
	_rootLoc = false;
	_method = "";
	_code = 200;
	_posLocation = -1;	
	_maxBodySize = _server.getMaxBodySize();
	_host = _server.getHost();
	_allowedMethods = _server.getAllowedMethods();
	_errorPages = _server.getErrorPages();
	_index = "";
	_autoIndex = _server.getAutoIndex();
	_allowUpload = false;
	_uploadDir = "";
	_return = _server.getReturn();
	_cgi = false;
	_cgiConf = _server.getCgiConf();
	_serverNames = _server.getServerName();
	_connectionKeepAlive = true;
	_multipartHeaders.clear();
	_acceptedContent.clear();
	_boundary = "";
	_fileName = "";
	_cookiesEnv.clear();
}

/*	REQUEST LINE: method | URI | and protocolversion
	HEADERS: A series of key-value pairs, each on its own line.
	BLANK LINE: A line with no content, indicating the end of the headers.
	BODY: Optional, depending on the type of request (e.g., present in POST requests).
*/
// _____________  PARSING REQUEST  _____________ 
void	Request::parseRequest(std::vector<unsigned char> buffer, int bytesRead) 
{
	_buffer += std::string(buffer.begin(), buffer.begin() + bytesRead);
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
			std::cout << "\033[32;1mFINISH REQUEST PARSING\033[0m" << std::endl;
			requestValidations();
		}
	} catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
		_status = FINISH_PARSED;
	}
}

// _____________  PARSING REQUEST LINE  _____________ 
void	Request::parseRequestLine() 
{
	size_t posBuffer = _buffer.find(CRLF);
	if (posBuffer == std::string::npos)
		return ;
	createRequestLineVector(_buffer.substr(0, posBuffer));
	_buffer.erase(0, posBuffer + 2);
	_status = REQUEST_LINE_PARSED;
}

void Request::createRequestLineVector(std::string requestLineStr)
{
	std::stringstream ss(requestLineStr);
	std::string element;
	while (ss >> element) {
		this->_requestLine.push_back(element);
	}
	if (_requestLine.size() != 3) 
		sendBadRequestError("Request parsing error: invalid request line", 400);
	_method = _requestLine[0];
}

// _____________  PARSING HEADERS   _____________ 
void	Request::parseHeaders() {
	while (_buffer.find(CRLF) != std::string::npos){
		std::string line = _buffer.substr(0, _buffer.find(CRLF));
		if (line.size() == 0) {
			_status = HEADERS_PARSED;
			_buffer.erase(0, 2);
			break;
		}
		else {
			addHeaderToMap(line, _headers);
			_buffer.erase(0, line.size() + 2);
		}
	}
}

void	Request::addHeaderToMap(std::string& line, std::map<std::string, std::string>& map){
	size_t posColon = line.find(':');
	if (posColon == std::string::npos) 
		sendBadRequestError("Request parsing error: invalid headers", 400);
	std::string name = trim(line.substr(0, posColon));
	std::string value = trim(line.substr(posColon + 1));
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
			sendBadRequestError("Request parsing error: invalid Transfer-Encoding value parameter", 400);
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
void	Request::parseBodyByChunked(){
	int	sizeChunk = 0;
	do {
		size_t	posEndSIze = _buffer.find(CRLF);
		sizeChunk = findSizeChunk(posEndSIze);
		if (sizeChunk <= 0 || posEndSIze + sizeChunk + 4 > _buffer.length()) //final of chunk or chunk incompleted
			break ;
		size_t posEndLine = _buffer.find(CRLF, posEndSIze + 2 + sizeChunk);
		if (posEndLine == std::string::npos || posEndLine != posEndSIze + 2 + sizeChunk) //there is not CRLF at the end of chunk
			sendBadRequestError("Request parsing error: invalid Transfre encoding", 400);
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
		sendBadRequestError("Error Bad Request: Invalid shunked body", 400);
	uint64_t result = strToHex(line);
	return (result);
}

void	Request::manageLineChunk(size_t posEndSIze, int sizeChunk) {
	std::string line = _buffer.substr(posEndSIze + 2, sizeChunk);
	if (line.length() + _body.length() > _maxBodySize)
		sendBadRequestError("Request parsing error: Body is too large", 400);
	_body = _body + line;
	_buffer.erase(0, posEndSIze + sizeChunk + 4);
}

void	Request::parseBodyByContentLength() { 
	std::map<std::string, std::string>::iterator	itLength = this->_headers.find("Content-Length");
	long unsigned int contentLength  = std::strtol((*itLength).second.c_str(), NULL, 10);
	if (contentLength > _maxBodySize)
		sendBadRequestError("Request parsing error: Body Length greater than Max body size", 400);
	size_t i = 0;
	while (i < _buffer.length() && _body.length() < contentLength) {
		_body.push_back(_buffer.at(i));
		i++;
	}
	_buffer.erase(0, i);
	if (_body.length() == contentLength) {
		_status = BODY_PARSED;
		_buffer.clear();
	}
}

// _____________  MULTIPART FORM BODY   _____________ 
void	Request::manageMultipartForm(){
	std::map<std::string, std::string>::iterator it =  _headers.find("Content-Type");
	if (it != _headers.end() && it->second.find("multipart/form-data") != std::string::npos) {
		getBoundary();
		saveMultipartHeaders();
		updateMultipartBody();
		saveFileName();
	}
} 

void	Request::getBoundary() 
{
	std::string content = _headers.find("Content-Type")->second;
	size_t pos = content.find("boundary=");
	if (pos == std::string::npos) 
		sendBadRequestError("Request parsing error: invalid Content-Type parameter 1", 400);
	_boundary = content.substr(pos+9, content.length() - (pos+9));
}

void	Request::saveMultipartHeaders() 
{
	size_t pos = _body.find("--" + _boundary);
	if (pos == std::string::npos)
		sendBadRequestError("Request parsing error: invalid Content-Type parameter 2 ", 400);
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
		sendBadRequestError("Request parsing error: invalid Multipart headers", 301);
}

void	Request::updateMultipartBody() 
{
	size_t startBody = _body.find("\r\n\r\n") + 4;
	if (startBody == std::string::npos)
		sendBadRequestError("Request parsing error: invalid Content-Type parameter 3 ", 400);
	size_t finishBody = _body.find(_boundary, startBody);
	if (finishBody == std::string::npos)
		sendBadRequestError("Request parsing error: invalid Content-Type parameter4 ", 400);
	_body.erase(finishBody - 3);
	_body.erase(0, startBody);
}


//Content-Disposition: form-data; name="file"; filename="example.txt"
void	Request::saveFileName() 
{
	std::string content = _multipartHeaders.find("Content-Disposition")->second;
	size_t auxPos = content.find("filename");
	if (auxPos == std::string::npos)
		return;
	size_t initFilename = content.find('\"', auxPos);
	size_t endFilename = content.find('\"', initFilename + 1);
	_fileName = content.substr(initFilename + 1, endFilename - initFilename -1);
}

// _____________  VALIDTATE REQUEST  _____________ 
void Request::requestValidations()
{
	checkHost(); 	
	checkConnectionKeepAlive();
	manageAcceptedContent();
	managePath();
	checkProtocolHttp();
	checkAllowMethod();
	updateIndex();
	updateUploadDir();
}
 
void	Request::checkHost() 
{
	if (_headers.find("Host") == _headers.end())
		sendBadRequestError("Request parsing error: invalid Host", 400);
	std::string hostReq = _headers.find("Host")->second;
	int portReq = 8080;
	size_t posColon = hostReq.find(":");
	if (posColon != std::string::npos) {
		portReq  = ft_atoi(hostReq.substr(posColon + 1));
		hostReq = hostReq.substr(0, posColon);
	}
	if (portReq != _port)
		sendBadRequestError("Request parsing error: invalid Host", 400);
	if (hostReq == _host || hostReq == _server.getIpAdress())
		return; //it's ok
	std::vector<std::string>::iterator it = std::find(_serverNames.begin(), _serverNames.end(), hostReq);
	if (it == _serverNames.end())
		sendBadRequestError("Request parsing error: invalid Host", 400);
}

void	Request::checkConnectionKeepAlive() 
{
	if (_headers.find("Connection") != _headers.end() && _headers.find("Connection")->second == "close")
		_connectionKeepAlive = false;
}

void	Request::manageAcceptedContent() 
{
	if (_headers.find("Accept") == _headers.end())
		return ;
	std::vector<std::string> acceptVec = ft_split(_headers.find("Accept")->second, ",");
	for (unsigned int i=0; i < acceptVec.size(); i++) {
		size_t posSlash = acceptVec[i].find('/');
		if (posSlash == std::string::npos) 
			sendBadRequestError("Request parsing error: invalid Accept header", 400);
		std::string type = trim(acceptVec[i].substr(0, posSlash));
		std::string subtype = trim(acceptVec[i].substr(posSlash + 1));
		
		size_t posSemicolon = subtype.find(';');
		if (posSemicolon != std::string::npos) {
			subtype = subtype.substr(0, posSemicolon);
		}
		_acceptedContent.insert(std::make_pair(type, subtype));
	}
}

void Request::managePath() 
{
	checkQuery(); //TODO: pending to add to the body ?? NURIA
	urlDecode();
	checkLocation();
	updateInfoLocation();
	updatePath();
}

void	Request::checkQuery() 
{
	std::string url = _requestLine[1];
	std::string::size_type	posQuery;
	posQuery = url.find("?");
	if (posQuery != std::string::npos) {
		_query = url.substr(posQuery + 1);
		_path = url.substr(0, posQuery);
	}
	else {
		_path = url;
	}
}

void	Request::urlDecode()
{
	std::string	decoded;
	size_t		len = _path.size();

	for (size_t i = 0; i < len; i++)
	{
		if (_path[i] == '%')
		{
			decoded += static_cast<char>(strToHex(_path.substr(i + 1, 2)));
			i += 2;
		}
		else if (_path[i] == '+')
			decoded += ' ';
		else
			decoded += _path[i];
	}
	_path = decoded;
}

void	Request::checkLocation() 
{
	std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
	size_t	nEqualLocs = 0;
	size_t 	j = 0;

	std::vector<std::string> vecPath = ft_split(_path, "/");
	for (size_t i=0; i < vecLocations.size(); i++) {
		std::vector<std::string> vecLoc = ft_split(vecLocations[i].getUri(), "/");
		for (j = 0; j < vecLoc.size() && j < vecPath.size(); j++) {
			if (vecPath[j] != vecLoc[j])
				break;
		}
		if (j > nEqualLocs) {
			_posLocation = i;
			nEqualLocs = j;
		}
	}
}

void	Request::updateInfoLocation() 
{
	if (_posLocation < 0)
		return ;
	std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
	LocationConfig location = vecLocations[_posLocation];
	updateRoot();
	for (std::vector<std::string>::const_iterator it = location.getAllowedMethods().begin(); it != location.getAllowedMethods().end(); ++it) {
		if (std::find(_allowedMethods.begin(), _allowedMethods.end(), *it) ==  _allowedMethods.end()) {
			_allowedMethods.push_back(*it);
		}
	}
	std::map<int, std::pair<std::string, std::string> > errorLoc = location.getErrorPages();
	_errorPages.insert(errorLoc.begin(), errorLoc.end());
	if (location.getIndex() != "")
		_index=  vecLocations[_posLocation].getIndex();
	_autoIndex = vecLocations[_posLocation].getAutoIndex();
	_allowUpload = vecLocations[_posLocation].getAllowUpload();
	_uploadDir = vecLocations[_posLocation].getUploadDir();
	_return = vecLocations[_posLocation].getReturn();
	_cgiConf.insert(location.getCgiConf().begin(), location.getCgiConf().end());
}

void	Request::updateRoot() {
	std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
	LocationConfig location = vecLocations[_posLocation];
	if (location.getRoot() != "") {
		_root = location.getRoot();
		_rootLoc = true;
	}
} 

void Request::updatePath() 
{
	if (_return != "") {
		_path = _return;
		_code = 301;
		_connectionKeepAlive = false;
	}
	else {
		if (_posLocation >= 0 && _rootLoc == true) { //there is location and it has root 
			std::vector<LocationConfig> vecLocations = _server.getLocationConfig();
			std::string uri = vecLocations[_posLocation].getUri();
			_path.erase(0, uri.size());
		}
		if (_root[_root.size() - 1] == '/' && _path != "" && _path[0] == '/')
			_root.erase(_root.size() - 1, 1);
		_path = _root + _path;
		setCgi();
	}
}

void	Request::setCgi()
{
	struct stat path_stat;
	if (stat(_path.c_str(), &path_stat) == 0) {
		if (S_ISREG(path_stat.st_mode) && access(_path.c_str(), X_OK) == 0) {
			this->_cgi = true;
		}
		else {
		std::string::size_type	found = _path.find_last_of(".");
			if (found != std::string::npos) {
				std::string ext = _path.substr(found);
				if (_cgiConf.find(ext) != _cgiConf.end()) //if extension is available
					this->_cgi = true;
			}
		}
	}
	setCookies();
}

void Request::setCookies() 
{
	if (_headers.find("Cookie") == _headers.end())
		return; 
	std::string headerCookies = _headers.find("Cookie")->second;
	std::vector<std::string> vectCookiesEnv = ft_split(headerCookies, ";");
	for (std::vector<std::string>::iterator it = vectCookiesEnv.begin(); it != vectCookiesEnv.end(); it++)
		trim(*it);
}

void Request::checkAllowMethod(){
	std::vector<std::string> allowedMethodsVect = _server.getAllowedMethods();
	std::vector<std::string>::iterator it = std::find(allowedMethodsVect.begin(), allowedMethodsVect.end(), _requestLine.front()); 
	if (it == allowedMethodsVect.end())
		sendBadRequestError("Request parsing error: method not allowed", 405);
}

void Request::checkProtocolHttp(){
	if (strcmp(_requestLine[2].c_str(), "HTTP/1.1")) 
		sendBadRequestError("Request parsing error: invalid http version", 400);
}

void Request::updateIndex(){
	if (_index != "" ) {
		if (_path[_path.size() - 1] != '/')
			_index = '/' + _index;
		_index = _path + _index;
	}
}

void Request::updateUploadDir(){
	if (_uploadDir != "" ) {
		std::string::size_type	found = _path.find_last_of("/");
		if (found == std::string::npos)
			sendBadRequestError("Request parsing error: invalid request", 400);
		if (_uploadDir[0] == '/')
			_uploadDir.erase(0, 1);
		_uploadDir = _path.substr(0, found + 1) + _uploadDir;
	}
}

// _____________  SEND BAD REQUEST ERROR  _____________ 
void	Request::sendBadRequestError(std::string errMssg, int code) {
	throw std::runtime_error(errMssg);
	_code = code;
}
