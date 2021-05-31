#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <iostream>
#include <map>
#include <unordered_map>

struct request_line
{
	std::string method_token;
	std::string request_URI;
	std::string http_version;
};

class Request
{
	private:

		request_line _start_line;
		std::unordered_map<std::string, std::string> _headers;
		std::string _body = nullptr;

	public:
		Request();
		~Request();
};

#endif