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
		setProtocolVersion(std::string const &protocol_version);

		void
		setStatusCode(int status_code);

		void
		setReasonPhrase(std::string const &reason_phrase);

		void
		setBody(std::string body);

		void
		setHeader(std::string const &key, std::string const &value);

		void
		printMessage(std::ostream &flux) const;

		void
		badRequest(void);

		void
		methodNotAllowed(void);

		void
		notFound(void);
};

std::ostream &
operator<<(std::ostream &flux, Response const &response);

#endif