#include "RequestParser.hpp"
#include "Server.hpp"

bool
RequestParser::isEndSection(std::string &line, int index)
{
	if (RequestHandler::isEndLine(line, index))
	{
		if (RequestHandler::isEndLine(line, index + 2))
			return true;
	}
	return false;
}

int
RequestParser::parseMethodToken(RequestHandler &rh)
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
RequestParser::parseRequestURI(RequestHandler &rh, int position)
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
RequestParser::parseHttpVersion(RequestHandler &rh, int position)
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
RequestParser::parseStartLine(RequestHandler &rh)
{
	int request_position = 0;

	request_position = parseMethodToken(rh);
	request_position = parseRequestURI(rh, request_position);
	request_position = parseHttpVersion(rh, request_position);
	rh.getRequest()->set_start_line_initilized(true);
	return request_position;
}

int
RequestParser::getOneHeader(RequestHandler &rh, int position)
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
	if (isEndSection(rh.getBuffer(), index))
	{
		rh.getRequest()->set_header_initialized(true);
		index += CRLF;
	}
	return index + CRLF;
}

bool
has_body(RequestHandler &rh)
{
	std::stringstream ss;
	int content_length;

	ss << rh.getRequest()->get_header_value("Content-Length");
	ss >> content_length;
	if (content_length == 0 || rh.getRequest()->get_header_value("Transfer-Encoding") != "chunked")
		return false;
	return true;
}

int
RequestParser::parseHeaders(RequestHandler &rh)
{
	int index = rh.getIdx();

	index = getOneHeader(rh, index);
	if (rh.getRequest()->is_headers_initialized() == true)
		rh.getRequest()->set_request_finalized(!has_body(rh));
	return index;
}

bool
is_complete_line(std::string &line, int idx)
{
	size_t i = idx;

	while (i < line.length())
	{
		if (RequestHandler::isEndLine(line, i))
			return true;
		i++;
	}
	if (i == line.length())
		return true;
	return false;
}

int
RequestParser::getBodyWithContentLength(RequestHandler &rh, int index)
{
	std::stringstream ss;
	std::string body;
	int content_length;

	ss << rh.getRequest()->get_header_value("Content-Length");
	ss >> content_length;
	body.assign(rh.getBuffer(), index, content_length);
	rh.getRequest()->set_body(body);
	rh.getRequest()->set_request_finalized(true);
	return index + content_length;
}

int
RequestParser::getChunkOfBody(RequestHandler &rh, int index)
{
	std::string tmp(rh.getRequest()->get_body());
	std::string body;
	int sublen = 0;
	while(rh.getBuffer()[index])
	{
		index++;
		sublen++;
	}
	body.assign(rh.getBuffer(), rh.getIdx(), sublen);
	tmp += body;
	rh.getRequest()->set_body(tmp);
	return index;
}

int
RequestParser::parseBody(RequestHandler &rh)
{
	int index = rh.getIdx();
	
	if (rh.getRequest()->get_header_value("Content-Length") != "0")
		return getBodyWithContentLength(rh, index);
	else if (rh.getRequest()->get_header_value("Transfer-Encoding") == "chunked")
	{
		if (rh.getBuffer()[index] == 0)
		{
			rh.getRequest()->set_request_finalized(true);
			return index;
		}
		index = getChunkOfBody(rh, index);
	}
	else
	{
		std::string body;
		int sublen = 0;
		while (rh.getBuffer()[index] && !isEndSection(rh.getBuffer(), index))
		{
			index++;
			sublen++;
		}
		body.assign(rh.getBuffer(), rh.getIdx(), sublen);
		rh.getRequest()->set_body(body);
		rh.getRequest()->set_request_finalized(true);
	}
	return index;
}

void print_buffer(std::string str) // for debug purpose
{
	 std::cout << "BUFFER : ";
	 for(size_t i = 0; i < str.length(); i++)
	 {
	 	if (str[i] == '\r')
	 		std::cout << "\\r";
	 	else if (str[i] == '\n')
	 		std::cout << "\\n";
	 	else
	 		std::cout << str[i];
	 }
}

int
RequestParser::parseRequest(std::map<fd_t, RequestHandler*>::iterator requesthandler,
							Server *server)
{
	(void)server;

	if (requesthandler->second->getRequest()->is_request_finalized() == true)
		return 1;
	if (is_complete_line(requesthandler->second->getBuffer(), requesthandler->second->getIdx()))
	{
		if (requesthandler->second->getRequest()->is_start_line_initialized() == false)
			requesthandler->second->setIdx(parseStartLine(*requesthandler->second));
		else if (requesthandler->second->getRequest()->is_headers_initialized() == false)
			requesthandler->second->setIdx(parseHeaders(*requesthandler->second));
		else if (requesthandler->second->getRequest()->is_request_finalized() == false)
		{
			requesthandler->second->setIdx(parseBody(*requesthandler->second));
		}
		requesthandler->second->clearBuffer(requesthandler->second->getIdx());
		std::cout << "Complete line detected" << std::endl;
	}
	else
		return 0;
	return 0;
}
