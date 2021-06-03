#include "Response.hpp"

Response::Response(void)
{
	this->_start_line.status_code = 200;
	this->_start_line.reason_phrase = "";
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
Response::set_protocol_version(std::string const &protocol_version)
{
	this->_start_line.protocol_version = protocol_version;
}

void
Response::set_status_code(int status_code)
{
	this->_start_line.status_code = status_code;
}

void
Response::set_reason_phrase(std::string const &reason_phrase)
{
	this->_start_line.reason_phrase = reason_phrase;
}