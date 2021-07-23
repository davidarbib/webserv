#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "AHttpMessage.hpp"

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "webserv/1.0.0"

class Response : public AHttpMessage
{
	private:

		status_line	_start_line;
		bool		_error_lock;

	public:
		Response(void);
		Response(Response &cpy);
		Response &
		operator=(Response const &src);
		virtual ~Response(void);

		void
		set_protocol_version(std::string const &protocol_version);

		void
		set_status_code(int status_code);

		void
		set_reason_phrase(std::string const &reason_phrase);

		void
		set_body(std::string body);

		void
		set_header(std::string const &key, std::string const &value);

		void
		print_message(std::ostream &flux) const;

		void
		bad_request(void);

		void
		method_not_allowed(void);

		void
		not_found(void);
};

std::ostream &
operator<<(std::ostream &flux, Response const &response);

#endif