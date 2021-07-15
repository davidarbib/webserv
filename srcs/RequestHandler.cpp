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
	std::cout << "after filling : " << std::endl << this->_in_buffer << std::endl;
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
