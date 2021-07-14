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
parseMethodToken(RequestHandler &rh)
{
	int index = 0;
	std::string method_token;

	while (rh.getBuffer()[index] && rh.getBuffer()[index] != ' ')
	{
		method_token += rh.getBuffer()[index];
		index++;
	}
	rh.getRequest()->set_method_token(method_token);
	return index + NEXT_SPACE_TOKEN;
}

int
parseRequestURI(RequestHandler &rh, int position)
{
	int index = position;
	std::string request_URI;

	while (rh.getBuffer()[index] && rh.getBuffer()[index] != ' ')
	{
		request_URI += rh.getBuffer()[index];
		index++;
	}
	rh.getRequest()->set_request_URI(request_URI);
	return index + NEXT_SPACE_TOKEN;
}

int
parseHttpVersion(RequestHandler &rh, int position)
{
	int index = position;
	std::string http_version;

	while (rh.getBuffer()[index] && !isEndLine(rh.getBuffer(), index))
	{
		http_version += rh.getBuffer()[index];
		index++;
	}
	rh.getRequest()->set_http_version(http_version);
	return index + CRLF;
}

int
parseStartLine(RequestHandler &rh)
{
	int request_position = 0;

	request_position = parseMethodToken(rh);
	request_position = parseRequestURI(rh, request_position);
	request_position = parseHttpVersion(rh, request_position);
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
		requesthandler->second->setIdx(parseStartLine(*requesthandler->second));
		requesthandler->second->setIdx(parseHeaders(requesthandler->second->getBuffer(), requesthandler->second->getRequest(), requesthandler->second->getIdx()));
		std::cout << "Parsing the line tututuuuuu...." << std::endl;
	}
	else
		return 0;
	return 0;
}
