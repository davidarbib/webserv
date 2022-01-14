#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "Request.hpp"
#include "Server.hpp"
#include "RequestHandler.hpp"
#include <sstream>
#include <string>
#include "webserv.hpp"

#define NEXT_SPACE_TOKEN 1
#define CRLF 2
#define CRLFCRLF 4

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
getOneHeader(RequestHandler &rh);

int
parseHeaders(RequestHandler &rh);

int
parseBody(RequestHandler &rh);

int
parseRequest(Connection *raw_request, Server &server, TicketsType &tickets, ReqHandlersType &RequestHandler);

int
getBodyWithContentLength(RequestHandler &rh, int index);

int
getChunkOfBody(RequestHandler &rh, int index);

#endif
