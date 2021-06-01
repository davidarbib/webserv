#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <iostream>
#include <map>

#define METHOD_NB 4

struct request_line
{
	std::string method_token;
	std::string request_URI;
	std::string http_version;
};

class Request
{
	private:

		std::string _method_list[METHOD_NB];
		request_line _start_line;
		std::map<std::string, std::string> _headers;
		std::string _body;

		void
		init_method_list(void);

		bool
		is_valid_method(std::string const &method);

	public:
		Request(void);
		Request(Request &cpy);
		Request &
		operator=(Request const &src);
		~Request(void);

		void
		set_method_token(std::string const &method_token);

		void
		set_request_URI(std::string const &request_URI);

		void
		set_http_version(std::string const &http_version);

		void
		set_body(std::string const &body);

		void
		add_header(std::string const &key, std::string const &value);
};

#endif
