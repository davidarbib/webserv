#include "Request.hpp"

Request::Request(void) : _start_line_initialized(false),
						 _headers_initialized(false),
						 _request_finalized(false),
						 _valid(true)
{
	this->setHeader("Content-Length", "0");
	this->setHeader("Transfer-Encoding", std::string());
	this->_body = std::string();
}

Request::Request(Request const & cpy)
{
	*this = cpy;
}

Request &
Request::operator=(Request const &src)
{
	this->_start_line = src._start_line;
	this->_headers = src._headers;
	this->_body = src._body;
	this->_valid = src._valid;
	return *this;
}

Request::~Request(void) {}

bool
Request::isContentLengthCorrect(void) const
{
	std::string content_length = get_header_value("Content-Length");
    if (content_length != "0" && std::atoi(content_length.c_str()) <= 0)
        return false;
    for (size_t i = 0; i < content_length.size(); i++)
    {
        if (isdigit(content_length[i]) == 0)
            return false;
    }
    return true;
}

void
Request::setMethodToken(std::string const &method_token)
{
		this->_start_line.method_token = method_token;
}

void
Request::setRequestURI(std::string const &request_URI)
{
	this->_start_line.request_URI = request_URI;
}

void
Request::setHttpVersion(std::string const &http_version)
{
	this->_start_line.http_version = http_version;
	this->_start_line_initialized = true;
}

void
Request::setBody(std::string const& body)
{
	this->_body = body;
}

void
Request::setHeader(std::string const &key, std::string const &value)
{
	this->_headers[key] = value;
}

void
Request::printMessage(std::ostream &flux) const
{
	flux << "---------------------" << "Start line :" << "---------------------" << std::endl;
	if (this->_start_line_initialized)
		flux << this->_start_line.method_token << " " << this->_start_line.request_URI << " " << this->_start_line.http_version << std::endl;
	AHttpMessage::printMessage(flux);
}

bool
Request::has_body(void) const
{
	if (this->_headers.find("Content-Length")->second == "0" || this->_headers.find("Transfer-Encoding")->second.empty())
		return false;
	return true;
}

const request_line
Request::getStartLine(void) const
{	
	return this->_start_line;
}

void
Request::setHeaderInitialized(bool value)
{
	this->_headers_initialized = value;
}

void
Request::setStartLineInitialized(bool value)
{
	this->_start_line_initialized = value;
}

void
Request::setRequestFinalized(bool value)
{
	this->_request_finalized = value;
}

bool
Request::iStartLineInitialized(void) const
{
	return this->_start_line_initialized;
}

bool
Request::isHeadersInitialized(void) const
{
	return this->_headers_initialized;
}

bool
Request::isRequestFinalized(void) const
{
	return this->_request_finalized;
}

bool
Request::hadOctetInBody(char c)
{
	this->_body += c;
	return true;
}

std::ostream&
operator<<(std::ostream &flux, Request const &request)
{
	request.printMessage(flux);
	return flux;
}


bool
Request::getValid(void) const
{
	return _valid;
}

void
Request::setValid(bool value)
{
	_valid = value;
}