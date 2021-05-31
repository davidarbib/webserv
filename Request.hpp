#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <iostream>
#include <map>

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
		std::map<std::string, std::string> _headers;
		std::string _body;

	public:
		Request(std::string raw_request);
		~Request(void);

		void
		set_method_token(std::string const &method_token);
		void
		set_request_URI(std::string const &request_URI);
		void
		set_http_version(std::string const &http_version);
		void
		set_body(std::string const &body);

};

#endif