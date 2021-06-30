#include "request_parser.hpp"

bool
is_end_line(std::string &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool
is_end_section(std::string &line, int index)
{
	if (is_end_line(line, index))
	{
		if (is_end_line(line, index + 2))
			return true;
	}
	return false;
}

int
parse_method_token(std::string &raw_request, Request *request)
{
	int index = 0;
	std::string method_token;

	while (raw_request[index] && raw_request[index] != ' ')
	{
		method_token += raw_request[index];
		index++;
	}
	request->set_method_token(method_token);
	return index + NEXT_SPACE_TOKEN;
}

int
parse_request_URI(std::string &raw_request, Request *request, int position)
{
	int index = position;
	std::string request_URI;

	while (raw_request[index] && raw_request[index] != ' ')
	{
		request_URI += raw_request[index];
		index++;
	}
	request->set_request_URI(request_URI);
	return index + NEXT_SPACE_TOKEN;
}

int
parse_http_version(std::string &raw_request, Request *request, int position)
{
	int index = position;
	std::string http_version;

	while (raw_request[index] && !is_end_line(raw_request, index))
	{
		http_version += raw_request[index];
		index++;
	}
	request->set_http_version(http_version);
	return index + CRLF;
}

int
parse_start_line(std::string &raw_request, Request *request)
{
	int request_position = 0;

	request_position = parse_method_token(raw_request, request);
	request_position = parse_request_URI(raw_request, request, request_position);
	request_position = parse_http_version(raw_request, request, request_position);
	return request_position;
}

int
get_one_header(std::string &raw_request, Request *request, int position)
{
	int index = position;
	std::string key;
	std::string value;

	while (raw_request[index] && raw_request[index] != ':')
	{
		key += raw_request[index];
		index++;
	}
	index += CRLF;
	while (raw_request[index] && !is_end_line(raw_request, index))
	{
		value += raw_request[index];
		index++;
	}
	request->set_header(key, value);
	return index + CRLF;
}

int
parse_headers(std::string &raw_request, Request *request, int position)
{
	int index = position;

	while (raw_request[index] && !is_end_section(raw_request, index))
		index = get_one_header(raw_request, request, index);
	return index + CRLFCRLF;
}

void
parse_request(std::string &raw_request, Request *request)
{
	int raw_request_index = 0;

	if (request->get_header_value("Transfer-Encoding") != "chunked")
	{
		raw_request_index = parse_start_line(raw_request, request);
		raw_request_index = parse_headers(raw_request, request, raw_request_index);
	}
}
