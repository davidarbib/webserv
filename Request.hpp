#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include "AHttpMessage.hpp"
#include "Response.hpp"

#define METHOD_NB 4

class Request : public AHttpMessage
{
	private:

		std::string _method_list[METHOD_NB];
		request_line _start_line;
		Response _response;

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
		set_body(char *body);

		void
		set_header(std::string const &key, std::string const &value);

		request_line
		get_start_line(void);

		std::map<std::string, std::string>
		get_header(void);

		char *
		get_body(void);

		Response
		get_response(void);
};

#endif
