#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "AHttpMessage.hpp"
#include "SmartFile.hpp"
#include <sstream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

#define FGET_SIZE 42
#define EMPTY_STR ""
#define BUFFER_SIZE 1024
#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "webserv/1.0.0"
#define CRLF_str "\r\n"
#define CRLFCRLF_str "\r\n\r\n"
#define MAX_URI_SIZE 32000
#define MAX_HEADER_SIZE 32000
#define CONTINUE 100
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NO_CONTENT 204
#define MOVED_PERMANTLY 301
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define NOT_ALLOWED 405
#define PAYLOAD_TO_LARGE 413
#define URI_TO_LONG 414
#define INTERNAL_SERVER_ERROR 500 
#define NOT_IMPLEMENTED 501
#define VERSION_NOT_SUPPORTED 505

class Response : public AHttpMessage
{
	private:

		status_line	_start_line; 

		int
		buildBody(SmartFile const& path);

	public:
		static std::map<int, std::string> errors_code;
		static std::map<std::string, std::string> handled_extensions;
		Response(void);
		Response(Response &cpy);
		Response &
		operator=(Response const &src);
		virtual ~Response(void);

		static std::map<int, std::string>
		fillResponseCodes(void);

		static std::map<std::string, std::string>
		fillHandledExtensions(void);

		void
		setProtocolVersion(std::string const &protocol_version);

		void
		setStatusCode(int status_code);

		void
		setReasonPhrase(std::string const &reason_phrase);

		void
		setHeader(std::string const &key, std::string const &value);

		void
		printMessage(std::ostream &flux) const;

		void
		buildPreResponse(int code);

		std::string
		getFileExtension(std::string const &uri) const;

		AHttpMessage::body_type
		serialize_response(void);

		void
		searchForBody(int code, std::string const &body_path, std::string const &file_type);

		void
		serverErrorResponse(void);
};

std::ostream &
operator<<(std::ostream &flux, Response const &response);

#endif
