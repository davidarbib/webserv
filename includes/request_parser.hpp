#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "Request.hpp"
#include "Server.hpp"
#include <string>

#define NEXT_SPACE_TOKEN 1
#define CRLF 2
#define CRLFCRLF 4

bool
is_end_line(std::string &line, int index);

bool
is_end_section(std::string &line, int index);

int
parse_method_token(std::string &raw_request, Request *request);

int
parse_request_URI(std::string &raw_request, Request *request, int position);

int
parse_http_version(std::string &raw_request, Request *request, int position);

int
parse_start_line(std::string &raw_request, Request *request);

int
get_one_header(std::string &raw_request, Request *request, int position);

int
parse_headers(std::string &raw_request, Request *request, int position);

int
parse_request(std::string &raw_request, Request *request, int raw_request_index);

int
parseRequest(std::map<fd_t, std::string>::iterator raw_request, Server *server);

#endif