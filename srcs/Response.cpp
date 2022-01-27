#include "Response.hpp"

Response::Response(void) : _error_lock(false)
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
Response::setBody(std::string const& body)
{
	this->_body = body;
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
	AHttpMessage::printMessage(flux);
}

void
Response::buildPreResponse(int code, std::string const& body_path)
{
	this->_start_line.status_code = code;
	this->_start_line.reason_phrase = Response::errors_code.find(code)->second;
	this->_headers["Server"] = SERVER_VERSION;
	this->_headers["Date"] = getDate();
	this->_headers["Content-Type"] = "text/html";
	if (code != 400)
		this->_headers["Connection"] = "keep-alive";
	else this->_headers["Connection"] = "close";
	this->_error_lock = true;
	buildBody(body_path);
}

std::string
Response::serialize_response(void)
{
	std::stringstream status_line;

	status_line << _start_line.protocol_version;
	status_line << " ";
	status_line << _start_line.status_code;
	status_line << " ";
	status_line << _start_line.reason_phrase.append(CRLF_str);
	std::string serialized(status_line.str());

	hash_map::iterator it = _headers.begin();
	while (it != _headers.end())
	{
		serialized += it->first;
		serialized += ": ";
		serialized += it->second;
		serialized += CRLF_str;
		it++;
	}
	serialized += CRLF_str;
	serialized += _body;
	return serialized;
}

void
Response::buildBody(std::string const& path)
{
	std::ifstream web_page(path.c_str());
	int size = 0;
	if (web_page)
	{
		std::string line;
		while (getline(web_page, line))
			_body += line;
		size = _body.length();
		std::stringstream s;
		s << size;
		this->_headers["Content-Length"] =  s.str();
		web_page.close();
	}
	else
		return ;
}

std::ostream &
operator<<(std::ostream &flux, Response const &response)
{
	response.printMessage(flux);
	return flux;
}

std::map<int, std::string> Response::fillResponseCodes(void)
{
	std::map<int, std::string> codes;

	codes.insert(std::make_pair(200, "OK"));
	codes.insert(std::make_pair(400, "Bad Request"));
	codes.insert(std::make_pair(401, "Unauthorized"));
	codes.insert(std::make_pair(403, "Forbidden"));
	codes.insert(std::make_pair(404, "Not Found"));
	codes.insert(std::make_pair(405, "Not Allowed"));
	codes.insert(std::make_pair(505, "Version Not Supported"));

	return codes;
}

std::map<int, std::string> Response::errors_code = std::map<int, std::string>();
