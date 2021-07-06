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
