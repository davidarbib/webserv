#include "Buffer.hpp"

Buffer::Buffer():
	_idx(0),
	_buffer("")
{
}

Buffer::Buffer(Buffer const &src):
	_idx(0),
	_buffer(src._buffer)
{
}

Buffer::~Buffer(void)
{
}

void
Buffer::fillBuffer(char *raw_buffer)
{
	this->_buffer += const_cast<char*>(raw_buffer);
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
	return this->_buffer;
}

char const *
Buffer::getContent(void)
{
	return _buffer.data();
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
Buffer::clearBuffer()
{				
	size_t count;
	size_t current_size = _buffer.size();
	//std::cout << "idx : " << _idx << std::endl;
	//std::cout << "size : " << _buffer.size() << std::endl;
	if (_idx >= current_size)	
		_buffer.clear();
	else
	{
		count = current_size - _idx;
		_buffer.assign(_buffer.substr(_idx, count)); 
		_idx = 0;
	}
}

void
Buffer::append(std::string const &message)
{
	_buffer += message;
}

void
Buffer::eatData(size_t size)
{				
	size_t count;
	size_t current_size = _buffer.size();
	if (size >= current_size)
		clearBuffer();
	else
	{
		count = current_size - size;
		_buffer.assign(_buffer.substr(size, count)); 
	}
}

bool
Buffer::isEmpty(void)
{
	return _buffer.empty();
}
