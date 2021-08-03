#include "request_parser.hpp"
#include "Server.hpp"

bool
isEndSection(std::string &line, int index)
{
	if (RequestHandler::isEndLine(line, index))
	{
		if (RequestHandler::isEndLine(line, index + 2))
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

	while (rh.getBuffer()[index] && !RequestHandler::isEndLine(rh.getBuffer(), index))
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
getOneHeader(RequestHandler &rh, int position)
{
	int index = position;
	std::string key;
	std::string value;

	while (rh.getBuffer()[index] && rh.getBuffer()[index] != ':')
	{
		key += rh.getBuffer()[index];
		index++;
	}
	index += 2;
	while (rh.getBuffer()[index] && !RequestHandler::isEndLine(rh.getBuffer(), index))
	{
		value += rh.getBuffer()[index];
		index++;
	}
	rh.getRequest()->set_header(key, value);
	return index + CRLF;
}

int
parseHeaders(RequestHandler &rh)
{
	int index = rh.getIdx();

	if (isEndSection(rh.getBuffer(), index)) {
		rh.getRequest()->set_header_initialized(true);
		index += CRLFCRLF;
	}
	index = getOneHeader(rh, index);
	return index;
}

bool
is_complete_line(std::string &line, int idx)
{
	for (size_t i = idx; i < line.length(); i++)
	{
		if (RequestHandler::isEndLine(line, i))
			return true;
	}
	return false;
}

int
parseBody(RequestHandler &rh)
{
	int index = rh.getIdx();
	while (rh.getBuffer()[index] && !isEndSection(rh.getBuffer(), index))
	{
		rh.getRequest()->hadOctetInBody(rh.getBuffer()[index]);
		index++;
	}
	return index;
}

int
parseRequest(std::map<fd_t, RequestHandler*>::iterator requesthandler, Server *server)
{
	(void)server;

	if (is_complete_line(requesthandler->second->getBuffer(), requesthandler->second->getIdx()))
	{
		if (requesthandler->second->getRequest()->is_start_line_initialized() == false)
			requesthandler->second->setIdx(parseStartLine(*requesthandler->second));
		else if (requesthandler->second->getRequest()->is_headers_initialized() == false)
			requesthandler->second->setIdx(parseHeaders(*requesthandler->second));
		else
		{
			requesthandler->second->setIdx(parseBody(*requesthandler->second));
		}
		requesthandler->second->clearBuffer(requesthandler->second->getIdx());
		// requesthandler->second->getRequest()->print_message(std::cout);
		std::cout << requesthandler->second->getBuffer();
	}
	else
	{
		return 0;
	}
	return 0;
}
