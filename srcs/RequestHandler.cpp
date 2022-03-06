#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Request *request, Connection *connection):
	_request(request),
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

Request *
RequestHandler::getRequest(void) const
{
	return this->_request;
}

void
RequestHandler::clearRequest(void)
{
	delete _request;
}

void
RequestHandler::clearBuffer(void)
{
	_connection.getInBuffer().clearBuffer();
}
