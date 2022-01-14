#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Connection *connection):
	_request(new Request),
	_connection(*connection)
{
}

RequestHandler::RequestHandler(RequestHandler const &src):
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
	_connection.getInBuffer().fillBuffer(raw_buffer);
}

void
RequestHandler::setIdx(int value)
{
	_connection.getInBuffer().setIdx(value);
}

void
RequestHandler::incIdx(int value)
{
	_connection.getInBuffer().incIdx(value);
}

int
RequestHandler::getIdx(void) const
{
	return _connection.getInBuffer().getIdx();
}

std::string &
RequestHandler::getBuffer(void)
{
	return _connection.getInBuffer().getBuffer();
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
	_connection.getInBuffer().clearBuffer(index);
}
