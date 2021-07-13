#include "RequestHandler.hpp"

RequestHandler::RequestHandler(std::string &in_buffer):
	_idx(0), 
	_in_buffer(in_buffer),
	_state(start),
	_request(NULL)
{
}

RequestHandler::RequestHandler(RequestHandler const &src):
	_idx(0), 
	_in_buffer(src._in_buffer),
	_state(start),
	_request(NULL)
{
}

RequestHandler::~RequestHandler(void)
{
}

void
RequestHandler::fillBuffer(char *raw_buffer)
{
	this->_in_buffer += const_cast<char*>(raw_buffer);
	std::cout << "after filling : " << std::endl << this->_in_buffer << std::endl;
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
RequestHandler::getBuffer(void) const
{
	return this->_in_buffer;
}

Request *
RequestHandler::getRequest(void) const
{
	return this->_request;
}
