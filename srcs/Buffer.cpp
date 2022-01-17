#include "Buffer.hpp"

Buffer::Buffer():
	_idx(0),
	_in_buffer("")
{
}

Buffer::Buffer(Buffer const &src):
	_idx(0),
	_in_buffer(src._in_buffer)
{
}

Buffer::~Buffer(void)
{
}

Buffer &
Buffer::operator=(Buffer const &rhs)
{
	this->_idx = rhs._idx;
	this->_in_buffer = rhs._in_buffer;
	return *this;
}

void
Buffer::fillBuffer(char *raw_buffer)
{
	this->_in_buffer += const_cast<char*>(raw_buffer);
}

void
Buffer::setIdx(int value)
{
	this->_idx = value;
}

void
Buffer::incIdx(int value)
{
	this->_idx += value;
}

int
Buffer::getIdx(void) const
{
	return this->_idx;
}

std::string &
Buffer::getBuffer(void)
{
	return this->_in_buffer;
}

bool
Buffer::isEndLine(std::string &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

void
Buffer::clearBuffer(int index)
{
	for (int i = 0; i < index; i++)
		this->_in_buffer[i] = 0;
}
