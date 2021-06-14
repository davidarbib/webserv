#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "AHttpMessage.hpp"

#define HTTP_VERSION "HTTP/1.1"

class Response : public AHttpMessage
{
	private:

		status_line	_start_line;

	public:
		Response(void);
		Response(Response &cpy);
		Response &
		operator=(Response const &src);
		~Response(void);

		void
		set_protocol_version(std::string const &protocol_version);

		void
		set_status_code(int status_code);

		void
		set_reason_phrase(std::string const &reason_phrase);

		void
		set_body(char *body);

		void
		set_header(std::string const &key, std::string const &value);

		void
		bad_request_response(void);
};

#endif