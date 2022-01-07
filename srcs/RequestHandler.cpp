#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Connection *connection, Buffer &buffer):
	_in_buffer(buffer),
	_request(new Request),
	_connection(connection)
{
}

RequestHandler::RequestHandler(RequestHandler const &src):
	_in_buffer(src._in_buffer),
	_request(src._request),
	_connection(src._connection)
{
}

RequestHandler::~RequestHandler(void)
{
	delete this->_request;
}

void
RequestHandler::fillBuffer(char *raw_buffer)
{
	_in_buffer.fillBuffer(raw_buffer);
}

void
RequestHandler::setIdx(int value)
{
	_in_buffer.setIdx(value);
}

void
RequestHandler::incIdx(int value)
{
	_in_buffer.incIdx(value);
}

int
RequestHandler::getIdx(void) const
{
	return _in_buffer.getIdx();
}

std::string &
RequestHandler::getBuffer(void)
{
	return _in_buffer.getBuffer();
}

Request *
RequestHandler::getRequest(void) const
{
	return this->_request;
}

bool
RequestHandler::isEndLine(std::string &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

void
RequestHandler::clearBuffer(int index)
{
	_in_buffer.clearBuffer(index);
}
