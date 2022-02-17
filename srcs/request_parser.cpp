#include "request_parser.hpp"

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

	while (rh.getBuffer()[index] && !isEndLine(rh.getBuffer(), index))
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

	while (rh.getBuffer()[index] && rh.getBuffer()[index] != ':')
	{
		key += rh.getBuffer()[index];
		index++;
	}
	index += 2;
	while (rh.getBuffer()[index] && !isEndLine(rh.getBuffer(), index))
	{
		//std::cout << "len : " << rh.getBuffer().size() << std::endl;
		//std::cout << "index : " << index << std::endl;
		value += rh.getBuffer()[index];
		index++;
	}
	rh.getRequest()->setHeader(key, value);
	if (isEndSection(rh.getBuffer(), index))
	{
		rh.getRequest()->setHeaderInitialized(true);
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
		if (isEndLine(line, i))
			return true;
		i++;
	}
//	if (i == line.length())
//		return true;
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
	if (is_complete_line(rh.getBuffer(), rh.getIdx()) && !rh.getBuffer().empty())
	{
		print_buffer(rh.getBuffer()); // for debug purpose
		if (rh.getRequest()->iStartLineInitialized() == false)
			rh.setIdx(parseStartLine(rh));
		else if (rh.getRequest()->isHeadersInitialized() == false)
		{
			std::cout << "start line" << std::endl;
			//print_buffer(rh.getBuffer()); // for debug purpose
			//std::cout << *rh.getRequest() << std::endl;
			rh.setIdx(parseHeaders(rh));
		}
		else if (rh.getRequest()->isRequestFinalized() == false)
		{
			std::cout << "headers initialized" << std::endl;
			//std::cout << "body parsing" << std::endl;
			rh.setIdx(parseBody(rh));
		}
		//print_buffer(rh.getBuffer()); // for debug purpose
		rh.clearBuffer();
		if (rh.getRequest()->isRequestFinalized() == true)
		{
			std::cout << "request finalized" << std::endl;
			//UNCOMENT TO SEE REQUEST INFOS
			//std::cout << *rh.getRequest() << std::endl;
			Ticket my_ticket(*raw_request, rh.getRequest(), server, it);
			tickets.push(my_ticket);
		}
	}
	else
		return 0;
	return 0;
}
