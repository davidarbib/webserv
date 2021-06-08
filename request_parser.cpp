#include "Request.hpp"

bool
is_end_line(const char *line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool
is_end_section(char *line, int index)
{
	if (is_end_line(line, index))
	{
		if (is_end_line(line, index + 2))
			return true;
	}
	return false;
}

int
parse_method_token(const char *raw_request, Request *request)
{
	int index = 0;
	std::string method_token;

	while (raw_request[index] && raw_request[index] != ' ')
	{
		method_token += raw_request[index];
		index++;
	}
	request->set_method_token(method_token);
	return index + 1;
}

int
parse_request_URI(const char *raw_request, Request *request, int position)
{
	int index = position;
	std::string request_URI;

	while (raw_request[index] && raw_request[index] != ' ')
	{
		request_URI += raw_request[index];
		index++;
	}
	request->set_request_URI(request_URI);
	return index + 1;
}

int
parse_http_version(const char *raw_request, Request *request, int position)
{
	int index = position;
	std::string http_version;

	while (raw_request[index] && !is_end_line(raw_request, index))
	{
		http_version += raw_request[index];
		index++;
	}
	request->set_http_version(http_version);
	return index + 2;
}

int
parse_start_line(const char *raw_request, Request *request)
{
	int request_position = 0;

	request_position = parse_method_token(raw_request, request);
	request_position = parse_request_URI(raw_request, request, request_position);
	request_position = parse_http_version(raw_request, request, request_position);
	return request_position;
}

void
parse_request(const char *raw_request, Request *request)
{
	int raw_request_index = 0;

	raw_request_index = parse_start_line(raw_request, request);
}
