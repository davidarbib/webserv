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

void
Response::set_body(std::string body)
{
	this->_body = body;
}

void
Response::set_header(std::string const &key, std::string const &value)
{
	this->_headers[key] = value;
}

void
Response::print_message(std::ostream &flux) const
{
	flux << "---------------------" << "Start line :" << "---------------------" << std::endl;
	flux << this->_start_line.protocol_version << " " << this->_start_line.status_code << " " << this->_start_line.reason_phrase << std::endl;
	AHttpMessage::print_message(flux);
}

void
Response::bad_request(void)
{
	if (this->_error_lock == false)
	{
		this->_start_line.status_code = 400;
		this->_start_line.reason_phrase = "Bad request";
		this->_headers["Content-Type"] = "text/html";
		this->_headers["Content-Length"] = "42";
		this->_headers["Server"] = SERVER_VERSION;
		this->_headers["Date"] = get_date();
		this->_headers["Connection"] = "close";
		this->_error_lock = true;
	}
}

void
Response::method_not_allowed(void)
{
	if (this->_error_lock == false)
	{
		this->_start_line.status_code = 405;
		this->_start_line.reason_phrase = "Not Allowed";
		this->_headers["Server"] = SERVER_VERSION;
		this->_headers["Date"] = get_date();
		this->_headers["Content-Type"] = "text/html";
		this->_headers["Content-Length"] = "42";
		this->_headers["Connection"] = "keep-alive";
		this->_error_lock = true;
	}
}

void
Response::not_found(void)
{
	if (this->_error_lock == false)
	{
		this->_start_line.status_code = 404;
		this->_start_line.reason_phrase = "Not Found";
		this->_headers["Server"] = SERVER_VERSION;
		this->_headers["Date"] = get_date();
		this->_headers["Content-Type"] = "text/html";
		this->_headers["Content-Length"] = "42";
		this->_headers["Connection"] = "keep-alive";
		this->_error_lock = true;
	}
}

std::ostream &
operator<<(std::ostream &flux, Response const &response)
{
	response.print_message(flux);
	return flux;
}