#include "RequestHandler.hpp"

RequestHandler::RequestHandler():
	_idx(0),
	_in_buffer(""),
	_state(start),
	_request(new Request)
{
}

RequestHandler::RequestHandler(RequestHandler const &src):
	_idx(0),
	_in_buffer(src._in_buffer),
	_state(start),
	_request(new Request)
{
}

RequestHandler::~RequestHandler(void)
{
	delete this->_request;
}

void
RequestHandler::fillBuffer(char *raw_buffer)
{
	this->_in_buffer += const_cast<char*>(raw_buffer);
}

void
RequestHandler::setIdx(int value)
{
	this->_idx = value;
}

void
RequestHandler::incIdx(int value)
{
	this->_idx += value;
}

int
RequestHandler::getIdx(void) const
{
	return this->_idx;
}

std::string &
RequestHandler::getBuffer(void)
{
	return this->_in_buffer;
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
	for (int i = 0; this->_in_buffer[i] && i < index; i++)
		this->_in_buffer[i] = 0;
}

void
RequestHandler::incState(void)
{
	if (this->_state != end)
		this->_state = static_cast<e_request_state>(static_cast<int>(this->_state) + 1);
}