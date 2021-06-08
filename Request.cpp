#include "Request.hpp"

Request::Request(void) : _response()
{
	this->_body = NULL;
	this->init_method_list();
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
Request::set_method_token(std::string const &method_token)
{
	if (this->is_valid_method(method_token) == true)
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
Request::set_body(char *body)
{
	this->_body = body;
}

void
Request::set_header(std::string const &key, std::string const &value)
{
	this->_headers[key] = value;
}

void
Request::init_method_list(void)
{
	this->_method_list[0] = "GET";
	this->_method_list[1] = "POST";
	this->_method_list[2] = "DELETE";
	this->_method_list[3] = "PUT";
}

bool
Request::is_valid_method(std::string const &method)
{
	for (int i = 0; i < METHOD_NB; i++)
	{
		if (method == this->_method_list[i])
			return true;
	}
	return false;
}

void
Request::print_request(std::ostream &flux) const
{
	std::map<std::string, std::string>::const_iterator it;
	flux << "---------------------" << "Start line :" << "---------------------" << std::endl;
	flux << this->_start_line.method_token << " " << this->_start_line.request_URI << " " << this->_start_line.http_version << std::endl;
	flux << "---------------------" << "Headers :" << "---------------------" << std::endl;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		flux << it->first << ": " << it->second << std::endl;
	}
	flux << "---------------------" << "Body :" << "---------------------" << std::endl;
	if (this->_body)
	{
		for (int i = 0; this->_body[i]; i++)
		flux << this->_body[i];
	}
}

std::ostream& operator<<(std::ostream &flux, Request const &request)
{
	request.print_request(flux);
	return flux;
}
