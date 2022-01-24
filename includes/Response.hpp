#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "AHttpMessage.hpp"
#include <sstream>
#include <fstream>

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "webserv/1.0.0"
#define CRLF_str "\r\n"
#define CRLFCRLF_str "\r\n\r\n"

class Response : public AHttpMessage
{
	private:

		status_line	_start_line;
		bool		_error_lock; 

		void
		buildBody(std::string const& path);

	public:
		static std::map<int, std::string> errors_code;
		Response(void);
		Response(Response &cpy);
		Response &
		operator=(Response const &src);
		virtual ~Response(void);

		static std::map<int, std::string>
		fillResponseCodes(void);

		void
		setProtocolVersion(std::string const &protocol_version);

		void
		setStatusCode(int status_code);

		void
		setReasonPhrase(std::string const &reason_phrase);

		void
		setBody(std::string const& body);

		void
		setHeader(std::string const &key, std::string const &value);

		void
		printMessage(std::ostream &flux) const;

		void
		buildPreResponse(int code);

		std::string
		serialize_response(void);
};

std::ostream &
operator<<(std::ostream &flux, Response const &response);

#endif