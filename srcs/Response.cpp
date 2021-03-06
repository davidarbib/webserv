#include "Response.hpp"

Response::Response(void)
{
	this->_start_line.protocol_version = HTTP_VERSION;
}

Response::Response(Response &cpy)
{
	*this = cpy;
}

Response &
Response::operator=(Response const &src)
{
	this->_start_line = src._start_line;
	this->_headers = src._headers;
	this->_body = src._body;
	return *this;
}

Response::~Response(void)
{ }

void
Response::setProtocolVersion(std::string const &protocol_version)
{
	this->_start_line.protocol_version = protocol_version;
}

void
Response::setStatusCode(int status_code)
{
	this->_start_line.status_code = status_code;
}

void
Response::setReasonPhrase(std::string const &reason_phrase)
{
	this->_start_line.reason_phrase = reason_phrase;
}

void
Response::setHeader(std::string const &key, std::string const &value)
{
	this->_headers[key] = value;
}

void
Response::printMessage(std::ostream &flux) const
{
	flux << "---------------------" << "Start line :" << "---------------------" << std::endl;
	flux << this->_start_line.protocol_version << " " << this->_start_line.status_code << " " << this->_start_line.reason_phrase << std::endl;
//	AHttpMessage::printMessage(flux);
}

void
Response::buildPreResponse(int code)
{
	this->_start_line.status_code = code;
	this->_start_line.reason_phrase = Response::errors_code.find(code)->second;
	if (code != CONTINUE)
	{
		this->_headers["Server"] = SERVER_VERSION;
		this->_headers["Date"] = getDate();
		if (code != BAD_REQUEST)
			this->_headers["Connection"] = "keep-alive";
		else
			this->_headers["Connection"] = "close";
	}
}

AHttpMessage::body_type
Response::serialize_response(void)
{
	std::stringstream status_line;

	status_line << _start_line.protocol_version;
	status_line << " ";
	status_line << _start_line.status_code;
	status_line << " ";
	status_line << _start_line.reason_phrase.append(CRLF_str);
	AHttpMessage::body_type serialized;
	serialized.reserve(status_line.str().size());
	std::string tmp(status_line.str());
	serialized.insert(serialized.end(), tmp.begin(), tmp.end());
	hash_map::iterator it = _headers.begin();
	while (it != _headers.end())
	{
		serialized.insert(serialized.end(), it->first.begin(), it->first.end());
		serialized.push_back(':');
		serialized.push_back(' ');
		serialized.insert(serialized.end(), it->second.begin(), it->second.end());
		serialized.push_back('\r');
		serialized.push_back('\n');
		it++;
	}
	serialized.push_back('\r');
	serialized.push_back('\n');
	for (size_t i = 0; i < _body.size(); i++)
		serialized.push_back(_body[i]);
	return serialized;
}

int
Response::buildBody(SmartFile const& path)
{
	int size = 0;
	int ret;
	char line[BUFFER_SIZE];
	bzero(line, BUFFER_SIZE);
	_body.reserve(BUFFER_SIZE);
	while ((ret = path.gets(line, BUFFER_SIZE)) > 0)
	{
		for (int i = 0; i < ret; i++)
        	_body.push_back(line[i]);
		bzero(line, BUFFER_SIZE);
	}
	size = _body.size();
	std::stringstream s;
	s << size;
	if (size > 0)
		this->_headers["Content-Length"] =  s.str();
	return size;
}

std::ostream &
operator<<(std::ostream &flux, Response const &response)
{
	response.printMessage(flux);
	return flux;
}

std::string
Response::getFileExtension(std::string const &uri) const
{
	if (!uri.empty())
	{
		size_t  extension_begin = uri.find_last_of(".");
		if (extension_begin == std::string::npos) 
			return std::string();
		return uri.substr(extension_begin, uri.size());
	}
	return std::string();
}

std::map<int, std::string>
Response::fillResponseCodes(void)
{
	std::map<int, std::string> codes;

	codes.insert(std::make_pair(CONTINUE, "Continue"));
	codes.insert(std::make_pair(OK, "OK"));
	codes.insert(std::make_pair(CREATED, "CREATED"));
	codes.insert(std::make_pair(NO_CONTENT, "No Content"));
	codes.insert(std::make_pair(MOVED_PERMANTLY, "Moved permantly"));
	codes.insert(std::make_pair(BAD_REQUEST, "Bad Request"));
	codes.insert(std::make_pair(UNAUTHORIZED, "Unauthorized"));
	codes.insert(std::make_pair(FORBIDDEN, "Forbidden"));
	codes.insert(std::make_pair(NOT_FOUND, "Not Found"));
	codes.insert(std::make_pair(NOT_ALLOWED, "Not Allowed"));
	codes.insert(std::make_pair(PAYLOAD_TO_LARGE, "Payload To Large"));
	codes.insert(std::make_pair(URI_TO_LONG, "Uri To Long"));
	codes.insert(std::make_pair(INTERNAL_SERVER_ERROR, "Internal Server Error"));
	codes.insert(std::make_pair(NOT_IMPLEMENTED, "Not Implemented"));
	codes.insert(std::make_pair(VERSION_NOT_SUPPORTED, "Version Not Supported"));

	return codes;
}

std::map<std::string, std::string>
Response::fillHandledExtensions(void)
{
	std::map<std::string, std::string> extensions;

	extensions.insert(std::make_pair(".html" ,"text/html"));
	extensions.insert(std::make_pair(".php" ,"text/html"));
	extensions.insert(std::make_pair(".mp3" ,"audio/mp3"));
	extensions.insert(std::make_pair(".mp4" ,"video/mp4"));
	extensions.insert(std::make_pair(".ttf" ,"font/ttf"));
	extensions.insert(std::make_pair(".png" ,"image/png"));
	extensions.insert(std::make_pair("" ,"application/octet-stream"));
	return extensions;
}

void
Response::searchForBody(int code, std::string const &body_path, std::string const &file_type)
{
	try
	{
	  SmartFile smartfile(body_path, "r");
	  if (buildBody(smartfile) == 0)
	  {
		  if (code == OK)
		  {
			  this->_start_line.reason_phrase = "No Content";
			  this->_start_line.status_code = NO_CONTENT;
		  }
		  setHeader("Content-Length", "0");
	  }
	  else
	  {
		  if (file_type.empty())
			  setHeader("Content-Type", handled_extensions.find("")->second);
		  else
		  {
			  std::map<std::string, std::string>::iterator it = handled_extensions.find(file_type);
			  if (it == handled_extensions.end())
				  setHeader("Content-Type", handled_extensions.find("")->second);
			  else
				  setHeader("Content-Type", it->second);
		  }
	  }
	}
	catch (std::exception &e)
	{
		setHeader("Content-Length", "0");		
		return ;
	}
	if (body_path == "./autoindex.html")
	{
		if (std::remove("./autoindex.html") != 0)
			throw std::exception();
	}
}

void
Response::serverErrorResponse(void)
{
	this->_start_line.reason_phrase = "Internal Server Error";
	this->_start_line.status_code = INTERNAL_SERVER_ERROR;
	this->_headers["Server"] = SERVER_VERSION;
	this->_headers["Date"] = getDate();
	this->_headers["Connection"] = "close";
}

std::map<int, std::string> Response::errors_code;
std::map<std::string, std::string> Response::handled_extensions;
