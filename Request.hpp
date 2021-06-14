#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include "AHttpMessage.hpp"
#include "Response.hpp"

#define METHOD_NB 4
#define EMPTY_STRING ""

class Request : public AHttpMessage
{
	private:

		std::string _method_list[METHOD_NB];
		request_line _start_line;
		Response _response;

		void
		init_method_list(void);

		bool
		is_valid_method(std::string const &method) const;

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

		void
		print_message(std::ostream &flux) const;

		bool
		has_body(void) const;

		std::string
		get_header_value(std::string const &header_name) const;

		Response
		get_response(void);
};

std::ostream & 
operator<<(std::ostream &flux, Request const &request);

#endif
