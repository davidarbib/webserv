#include "request_parser.hpp"

void print_buffer(AHttpMessage::body_type buffer) // for debug purpose
{
	 std::cout << "BUFFER : " << std::endl;
	 for(size_t i = 0; i < buffer.size(); i++)
	 {
	 	if (buffer[i] == '\r')
	 		std::cout << "\\r";
	 	else if (buffer[i] == '\n')
	 		std::cout << "\\n";
	 	else
	 		std::cout << buffer[i];
	 }
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
	size_t index = static_cast<size_t>(position);
	std::string key;
	std::string value;

	while (index < rh.getBuffer().size() && rh.getBuffer()[index] != ':')
	{
		key += rh.getBuffer()[index];
		index++;
	}
	index += 2;
	while (index < rh.getBuffer().size() && !isEndLine(rh.getBuffer(), index))
	{
		value += rh.getBuffer()[index];
		index++;
	}
	if (key.empty() || value.empty())
		rh.getRequest()->setValid(false);
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

	ss << rh.getRequest()->getHeaderValue("Content-Length");
	ss >> content_length;
	if (content_length == 0 && rh.getRequest()->getHeaderValue("Transfer-Encoding") != "chunked")
		return false;
	else if (rh.getRequest()->isContentLengthCorrect())
		return true;
	return false;
}

int
parseHeaders(RequestHandler &rh)
{
	int index = 0;

	if (rh.getBuffer().size() == 2 && rh.getBuffer()[0] == '\r' && rh.getBuffer()[1] == '\n')
	{
		rh.getRequest()->setHeaderInitialized(true);
		return index + CRLF;
	}
	else
		index = getOneHeader(rh, index);
	if (rh.getRequest()->isHeadersInitialized() == true)
		rh.getRequest()->setRequestFinalized(!has_body(rh));
	return index;
}

bool
isCompleteLine(AHttpMessage::body_type &line)
{
	size_t i = 0;

	while (i < line.size())
	{
		if (isEndLine(line, i))
			return true;
		i++;
	}
	return false;
}

int
getBodyWithContentLength(RequestHandler &rh, int index)
{
	std::stringstream ss;
	AHttpMessage::body_type body;
	size_t content_length;

	ss << rh.getRequest()->getHeaderValue("Content-Length");
	ss >> content_length;
	body.reserve(content_length);
	for (AHttpMessage::body_type::iterator it = rh.getBuffer().begin(); it != rh.getBuffer().end(); it++)
	{
		body.push_back(*it);
		index++;
	}
	rh.getRequest()->setBody(body);
	if (rh.getRequest()->getBody().size() == content_length)
		rh.getRequest()->setRequestFinalized(true);
	return index;
}

int
getChunkOfBody(RequestHandler &rh, int index)
{
	unsigned int i = index;
	std::string tmp;
	size_t chunk_size = 1;
	AHttpMessage::body_type chunk;
	
	while (!isEndLine(rh.getBuffer(), index))
		index++;
	chunk.insert(chunk.end(), rh.getBuffer().begin() + i, rh.getBuffer().begin() + index);
	std::stringstream ss;
	ss << std::hex << tmp;
	ss >> chunk_size;
	if (chunk_size == 0)
	{
		rh.getRequest()->setRequestFinalized(true);
		return index;
	}
	index += CRLF;
	chunk.clear();
	chunk.insert(chunk.end(), rh.getBuffer().begin() + index, rh.getBuffer().begin() + chunk_size);
	rh.getRequest()->setBody(chunk);
	index += CRLF;
	return index += chunk_size;
}

bool
isCompleteChunk(RequestHandler &rh)
{
	int match_end_line = 0;
	for (size_t index = 0; index < rh.getBuffer().size() && match_end_line < 2; index++)
	{
		if (isEndLine(rh.getBuffer(), index))
			match_end_line++;
	}
	if (match_end_line >= 2)
		return true;
	return false;
}

int
parseBody(RequestHandler &rh)
{
	int index = 0;
	
	if (rh.getRequest()->getHeaderValue("Content-Length") != "0")
		return getBodyWithContentLength(rh, index);
	else if (rh.getRequest()->getHeaderValue("Transfer-Encoding") == "chunked")
	{
		if (isCompleteChunk(rh))
			index = getChunkOfBody(rh, index);
	}
	else
	{
		AHttpMessage::body_type body;
		int sublen = 0;
		while (rh.getBuffer()[index] && !isEndSection(rh.getBuffer(), index))
		{
			index++;
			sublen++;
		}
		body.insert(body.begin(),  rh.getBuffer().begin() + rh.getIdx(), rh.getBuffer().begin() + sublen);
		rh.getRequest()->setBody(body);
		rh.getRequest()->setRequestFinalized(true);
	}
	return index;
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
	if (isCompleteLine(rh.getBuffer()) || rh.getRequest()->isHeadersInitialized() == true)
	{
		if (rh.getRequest()->iStartLineInitialized() == false)
			rh.setIdx(parseStartLine(rh));
		else if (rh.getRequest()->isHeadersInitialized() == false && rh.getRequest()->getValid())
			rh.setIdx(parseHeaders(rh));
		else if (rh.getRequest()->isRequestFinalized() == false && rh.getRequest()->getValid())
		{
			rh.setIdx(parseBody(rh));
			//print_buffer(rh.getBuffer()); // for debug purpose
		}
		rh.clearBuffer();
		if (rh.getRequest()->isRequestFinalized() == true || !rh.getRequest()->getValid())
		{
			//UNCOMENT TO SEE REQUEST INFOS
			//std::cout << *rh.getRequest() << std::endl;
			Ticket my_ticket(*raw_request, rh.getRequest(), server);
			tickets.push(my_ticket);
			request_handlers.erase(it);	
		}
	}
	return 0;
}
