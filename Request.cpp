#include "Request.hpp"

Request::Request(void) {}

Request::Request(Request & src)
{
	*this = src;
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
Request::set_method_token(std::string const &method_token)
{
	this->_start_line.method_token = method_token;
}

void
Request::set_request_URI(std::string const &request_URI)
{
	this->_start_line.request_URI = request_URI;
}

void
Request::set_http_version(std::string const &http_version)
{
	this->_start_line.http_version = http_version;
}

void
Request::set_body(std::string const &body)
{
	this->_body = body;
}

void
Request::add_header(std::string const &key, std::string const &value)
{
	this->_headers[key] = value;
}
