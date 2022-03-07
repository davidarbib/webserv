#include "RequestHandler.hpp"

RequestHandler::RequestHandler(RequestIt request_it, Connection *connection):
	_request_it(request_it),
	_connection(*connection)
{
}

RequestHandler::RequestHandler(RequestHandler const &src):
	_request_it(src._request_it),
	_connection(src._connection)
{
}

RequestHandler::~RequestHandler(void)
{
}

void
RequestHandler::fillBuffer(char *raw_buffer, int size)
{
	_connection.getInBuffer().fillBuffer(raw_buffer, size);
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

std::vector<char> &
RequestHandler::getBuffer(void)
{
	return _connection.getInBuffer().getBuffer();
}

RequestIt
RequestHandler::getRequest(void) const
{
	return _request_it;
}

void
RequestHandler::clearBuffer(void)
{
	_connection.getInBuffer().clearBuffer();
}

void
RequestHandler::clearRequest(void)
{
}
