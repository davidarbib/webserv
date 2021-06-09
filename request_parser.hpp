#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "Request.hpp"
#include <string>

bool
is_end_line(const char *line, int index);

bool
is_end_section(const char *line, int index);

int
parse_method_token(const char *raw_request, Request *request);

int
parse_request_URI(const char *raw_request, Request *request, int position);

int
parse_http_version(const char *raw_request, Request *request, int position);

int
parse_start_line(const char *raw_request, Request *request);

int
get_one_header(const char *raw_request, Request *request, int position);

int
parse_headers(const char *raw_request, Request *request, int position);

void
parse_request(const char *raw_request, Request *request);

#endif