#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>

struct status_line
{
	std::string protocol_version;
	int 		status_code;
	std::string	reason_phrase;		
};

class Response
{
	private:

		status_line	start_line;
		std::map<std::string, std::string> _headers;
		std::string _body;
};

#endif