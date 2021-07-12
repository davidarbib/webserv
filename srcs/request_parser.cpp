#include "request_parser.hpp"
#include "Server.hpp"

bool
isEndLine(std::string &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool
isEndSection(std::string &line, int index)
{
	if (isEndLine(line, index))
	{
		if (isEndLine(line, index + 2))
			return true;
	}
	return false;
}

int
parseMethodToken(std::string &raw_request, Request *request)
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
parseRequestURI(std::string &raw_request, Request *request, int position)
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
parseHttpVersion(std::string &raw_request, Request *request, int position)
{
	int index = position;
	std::string http_version;

	while (raw_request[index] && !isEndLine(raw_request, index))
	{
		http_version += raw_request[index];
		index++;
	}
	request->set_http_version(http_version);
	return index + CRLF;
}

int
parseStartLine(std::string &raw_request, Request *request)
{
	int request_position = 0;

	request_position = parseMethodToken(raw_request, request);
	request_position = parseRequestURI(raw_request, request, request_position);
	request_position = parseHttpVersion(raw_request, request, request_position);
	return request_position;
}

int
getOneHeader(std::string &raw_request, Request *request, int position)
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
	while (raw_request[index] && !isEndLine(raw_request, index))
	{
		value += raw_request[index];
		index++;
	}
	request->set_header(key, value);
	return index + CRLF;
}

int
parseHeaders(std::string &raw_request, Request *request, int position)
{
	int index = position;

	while (raw_request[index] && !isEndSection(raw_request, index))
		index = getOneHeader(raw_request, request, index);
	return index + CRLFCRLF;
}

bool
is_complete_line(std::string &line, int idx)
{
	for (size_t i = idx; i < line.length(); i++)
	{
		if (isEndLine(line, i))
			return true;
	}
	return false;
}

int
parseRequest(std::map<fd_t, RequestHandler*>::iterator requesthandler, Server *server)
{
	(void)server;

	if (is_complete_line(requesthandler->second->getBuffer(), requesthandler->second->getIdx()))
	{
		requesthandler->second->setIdx(parseStartLine(requesthandler->second->getBuffer(), requesthandler->second->getRequest()));
		requesthandler->second->setIdx(parseHeaders(requesthandler->second->getBuffer(), requesthandler->second->getRequest(), requesthandler->second->getIdx()));
		std::cout << "Parsing the line tututuuuuu...." << std::endl;
	}
	else
		return 0;
	return 0;
}
