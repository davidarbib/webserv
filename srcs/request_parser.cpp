#include "request_parser.hpp"

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
	rh.getRequest()->setMethodToken(method_token);
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
	rh.getRequest()->setRequestURI(request_URI);
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
	rh.getRequest()->setHttpVersion(http_version);
	return index + CRLF;
}

int
parseStartLine(RequestHandler &rh)
{
	int request_position = 0;

	request_position = parseMethodToken(rh);
	request_position = parseRequestURI(rh, request_position);
	request_position = parseHttpVersion(rh, request_position);
	rh.getRequest()->setStartLineInitialized(true);
	return request_position;
}

int
getOneHeader(RequestHandler &rh, int position)
{
	int index = position;
	std::string key;
	std::string value;

	if (rh.getBuffer()[rh.getIdx()] == 0)
		return index;
	rh.setHeaderAreParsed(false);
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
	rh.getRequest()->setHeader(key, value);
	if (isEndSection(rh.getBuffer(), index))
	{
		rh.getRequest()->setHeaderInitialized(true);
		index += CRLF;
	}
	rh.setHeaderAreParsed(true);
	return index + CRLF;
}

bool
has_body(RequestHandler &rh)
{
	std::stringstream ss;
	int content_length;

	ss << rh.getRequest()->get_header_value("Content-Length");
	ss >> content_length;
	if (content_length == 0 && rh.getRequest()->get_header_value("Transfer-Encoding") != "chunked")
		return false;
	return true;
}

int
parseHeaders(RequestHandler &rh)
{
	int index = rh.getIdx();

	index = getOneHeader(rh, index);
	if (rh.getRequest()->isHeadersInitialized() == true)
		rh.getRequest()->setRequestFinalized(!has_body(rh));
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
	return false;
}

int
getBodyWithContentLength(RequestHandler &rh, int index)
{
	std::stringstream ss;
	std::string body;
	int content_length;

	ss << rh.getRequest()->get_header_value("Content-Length");
	ss >> content_length;
	body.assign(rh.getBuffer(), index, content_length);
	rh.getRequest()->setBody(body);
	rh.getRequest()->setRequestFinalized(true);
	return index + content_length;
}

int
getChunkOfBody(RequestHandler &rh, int index)
{
	std::string tmp(rh.getRequest()->getBody());
	std::string body;
	int sublen = 0;
	while(rh.getBuffer()[index])
	{
		index++;
		sublen++;
	}
	body.assign(rh.getBuffer(), rh.getIdx(), sublen);
	tmp += body;
	rh.getRequest()->setBody(tmp);
	return index;
}

int
parseBody(RequestHandler &rh)
{
	int index = rh.getIdx();
	
	if (rh.getRequest()->get_header_value("Content-Length") != "0")
		return getBodyWithContentLength(rh, index);
	else if (rh.getRequest()->get_header_value("Transfer-Encoding") == "chunked")
	{
		if (rh.getBuffer()[index] == 0)
		{
			rh.getRequest()->setRequestFinalized(true);
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
		rh.getRequest()->setBody(body);
		rh.getRequest()->setRequestFinalized(true);
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
parseRequest(Connection *raw_request, Server &server, TicketsType &tickets, ReqHandlersType &request_handlers)
{
	ReqHandlersType::iterator it = request_handlers.find(raw_request->getSocketFd());
	if (it == request_handlers.end())
	{
		Request *request = new Request();
		RequestHandler new_rh(request, raw_request);
		request_handlers.insert(std::make_pair(raw_request->getSocketFd(), new_rh));
		return 0;
	}
	RequestHandler &rh = it->second;
	if (rh.getRequest()->isRequestFinalized() == true)
		return 1;
	if (is_complete_line(rh.getBuffer(), rh.getIdx()))
	{
		if (rh.getRequest()->iStartLineInitialized() == false)
			rh.setIdx(parseStartLine(rh));
		else if (rh.getRequest()->isHeadersInitialized() == false)
		{
			std::cout << "header line initialized" << std::endl; //TODO
			rh.setIdx(parseHeaders(rh));
		}
		else if (rh.getRequest()->isRequestFinalized() == false)
		{
			std::cout << "parsing body" << std::endl; //TODO
			rh.setIdx(parseBody(rh));
		}
		// print_buffer(rh.getBuffer());
		rh.clearBuffer(rh.getIdx());
		if (rh.getRequest()->isRequestFinalized() == true)
		{
			std::cout << "request finalized" << std::endl; //TODO
			//UNCOMENT TO SEE REQUEST INFOS
			std::cout << *rh.getRequest() << std::endl;
			Ticket my_ticket(*raw_request, rh.getRequest(), server);
			tickets.push(my_ticket);
			rh.attachNewRequest();
		}
	}
	else
	{
		if (rh.getBuffer()[rh.getIdx() - 4] == '\r' && rh.getBuffer()[rh.getIdx() - 3] == '\n')
		{
			std::cout << "WALLAH C FINI WSH" << "PH" << std::endl; 
			rh.getRequest()->setHeaderInitialized(true);
			rh.incIdx(CRLF);
			rh.setHeaderAreParsed(false);
		}
		return 0;
	}
	return 0;
}
