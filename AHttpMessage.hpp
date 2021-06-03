#ifndef AHTTP_MESSAGE_HPP
#define AHTTP_MESSAGE_HPP

#include <string>
#include <map>

struct request_line
{
	std::string method_token;
	std::string request_URI;
	std::string http_version;
};

struct status_line
{
	std::string protocol_version;
	int 		status_code;
	std::string	reason_phrase;		
};

class AHttpMessage
{
	protected:

		std::map<std::string, std::string> 	_headers;
		char								*_body;		
};

#endif