#include "Request.hpp"

bool	is_end_line(char *line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool	is_end_section(char *line, int index)
{
	if (is_end_line(line, index))
	{
		if (is_end_line(line, index + 2))
			return true;
	}
	return false;
}

int		parse_method_token(const char *raw_request, Request *request)
{
	int index = 0;
	std::string method_token;

	while (raw_request[index] && raw_request[index] != ' ')
	{
		method_token += raw_request[index];
		index++;
	}
	request->set_method_token(method_token);
	return index;
}

void	parse_request(char *raw_request, Request *request)
{
	int request_position = 0;

	request_position = parse_method_token(raw_request, request);
}