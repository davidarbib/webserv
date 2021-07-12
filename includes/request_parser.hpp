#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "Request.hpp"
#include "Server.hpp"
#include <string>

#define NEXT_SPACE_TOKEN 1
#define CRLF 2
#define CRLFCRLF 4

bool
isEndLine(std::string &line, int index);

bool
isEndSection(std::string &line, int index);

int
parseMethodToken(std::string &raw_request, Request *request);

int
parseRequestURI(std::string &raw_request, Request *request, int position);

int
parseHttpVersion(std::string &raw_request, Request *request, int position);

int
parseStartLine(std::string &raw_request, Request *request);

int
getOneHeader(std::string &raw_request, Request *request, int position);

int
parseHeaders(std::string &raw_request, Request *request, int position);

int
parseRequest(std::map<fd_t, RequestHandler*>::iterator raw_request, Server *server);

#endif
