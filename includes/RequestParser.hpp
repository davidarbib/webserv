#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "Request.hpp"
#include "Server.hpp"
#include "RequestHandler.hpp"
#include <sstream>
#include <string>

#define NEXT_SPACE_TOKEN 1
#define CRLF 2
#define CRLFCRLF 4

class RequestParser
{

	public :
		RequestParser();

		virtual ~RequestParser();

		int
		parseRequest(Buffer &buffer, Server &server);

	private :	
		std::map<fd_t, RequestHandler*> request_handlers;

		RequestParser(RequestParser const &src);

		bool
		isEndSection(std::string &line, int index);

		int
		parseMethodToken(RequestHandler &rh);

		int
		parseRequestURI(RequestHandler &rh, int position);

		int
		parseHttpVersion(RequestHandler &rh, int position);

		int
		parseStartLine(RequestHandler &rh);

		int
		getOneHeader(RequestHandler &rh, int position);

		int
		parseHeaders(RequestHandler &rh);

		int
		parseBody(RequestHandler &rh);

		int
		getBodyWithContentLength(RequestHandler &rh, int index);

		int
		getChunkOfBody(RequestHandler &rh, int index);

};

#endif
