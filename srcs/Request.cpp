#include "Request.hpp"

Request::Request(void) : _start_line_initialized(false),
						 _headers_initialized(false),
						 _request_finalized(false) 
{
	this->setHeader("Content-Length", "0");
	this->setHeader("Transfer-Encoding", std::string());
	this->_body = std::string();
	this->initMethodList();
}

Request::Request(Request & cpy)
{
	*this = cpy;
}

Request &
Request::operator=(Request const &src)
{
	this->_start_line = src._start_line;
	this->_headers = src._headers;
	this->_body = src._body;
	return *this;
}

Request::~Request(void) {}

void
Request::setMethodToken(std::string const &method_token)
{
	if (this->isAllowedMethod(method_token) == true)
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
Request::initMethodList(void)
{
	this->_method_list[0] = "GET";
	this->_method_list[1] = "POST";
	this->_method_list[2] = "DELETE";
	this->_method_list[3] = "PUT";
}

bool
Request::isAllowedMethod(std::string const &method) const
{
	for (int i = 0; i < METHOD_NB; i++)
	{
		if (method == this->_method_list[i])
			return true;
	}
	return false;
}

bool
Request::isValidMethod(std::string const &method) const
{
	for (size_t i = 0; i < method.length(); i++)
	{
		if (std::isupper(method[i]) == 0)
			return false;
	}
	return true;
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

std::string
Request::get_header_value(std::string const &header_name) const
{
	hash_map::const_iterator it;

	it = this->_headers.find(header_name);
	if (it != this->_headers.end())
		return it->second;
	return std::string();
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
