#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <cctype>
#include "AHttpMessage.hpp"
#include "Response.hpp"

#define METHOD_NB 4

class Request : public AHttpMessage
{
	private:

		std::string 	_method_list[METHOD_NB];
		request_line 	_start_line;
		Response 		_response;
		bool			_start_line_initialized;
		bool			_headers_initialized;
		bool			_request_finalized;

		void
		init_method_list(void);

		bool
		is_allowed_method(std::string const &method) const;

		bool
		is_valid_method(std::string const &method) const;

	public:

		Request(void);
		Request(Request &cpy);
		Request &
		operator=(Request const &src);
		virtual ~Request(void);

		void
		set_method_token(std::string const &method_token);

		void
		set_request_URI(std::string const &request_URI);

		void
		set_http_version(std::string const &http_version);

		void
		set_body(std::string body);

		void
		set_header(std::string const &key, std::string const &value);

		void
		print_message(std::ostream &flux) const;

		void
		set_start_line_initilized(bool value);

		void
		set_header_initialized(bool value);

		bool
		is_start_line_initialized(void) const;

		bool
		is_headers_initialized(void) const;

		bool
		is_request_finalized(void) const;

		bool
		has_body(void) const;

		bool
		hadOctetInBody(char c);

		std::string
		get_header_value(std::string const &header_name) const;

		Response
		get_response(void);
};

std::ostream &
operator<<(std::ostream &flux, Request const &request);

#endif
