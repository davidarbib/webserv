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