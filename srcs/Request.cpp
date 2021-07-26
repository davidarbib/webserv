#include "Request.hpp"

Request::Request(void) : _response(), 
						 _start_line_initialized(false),
						 _headers_initialized(false),
						 _request_finalized(false) 
{
	this->set_header("Content-Length", "0");
	this->set_header("Transfer-Encoding", "");
	this->_body = std::string();
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
	if (this->is_allowed_method(method_token) == true)
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
	this->_start_line_initialized = true;
}

void
Request::set_body(std::string body)
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
Request::is_allowed_method(std::string const &method) const
{
	for (int i = 0; i < METHOD_NB; i++)
	{
		if (method == this->_method_list[i])
			return true;
	}
	return false;
}

bool
Request::is_valid_method(std::string const &method) const
{
	for (size_t i = 0; i < method.length(); i++)
	{
		if (std::isupper(method[i]) == 0)
			return false;
	}
	return true;
}

void
Request::print_message(std::ostream &flux) const
{
	flux << "---------------------" << "Start line :" << "---------------------" << std::endl;
	if (this->_start_line_initialized)
		flux << this->_start_line.method_token << " " << this->_start_line.request_URI << " " << this->_start_line.http_version << std::endl;
	AHttpMessage::print_message(flux);
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
	return EMPTY_STRING;
}

Response
Request::get_response(void)
{
	Response response(this->_response);
	return response;
}

void
Request::set_header_initialized(bool value)
{
	this->_headers_initialized = value;
}

bool
Request::is_start_line_initialized(void) const
{
	return this->_start_line_initialized;
}

bool
Request::is_headers_initialized(void) const
{
	return this->_headers_initialized;
}

bool
Request::is_request_finalized(void) const
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
	request.print_message(flux);
	return flux;
}