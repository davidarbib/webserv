#include "Buffer.hpp"

Buffer::Buffer():
	_idx(0)
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
Buffer::fillBuffer(char *raw_buffer, int size)
{
	size_t len = _buffer.size();
	_buffer.resize(len + size);
	for (int i = 0; i < size; i++)
		_buffer[len + i] = raw_buffer[i];
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

std::vector<char> &
Buffer::getBuffer(void)
{
	return _buffer;
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
	size_t current_size = _buffer.size();
	if (_idx >= current_size)	
		_buffer.clear();
	else
	{
		_buffer.assign(_buffer.begin() + _idx, _buffer.end());
		_idx = 0;
	}
}

void
Buffer::append(std::string const &message)
{
	_buffer.insert(_buffer.end(), message.begin(), message.end());
}

void
Buffer::append(std::vector<char> const &message)
{
	_buffer.insert(_buffer.end(), message.begin(), message.end());
}

bool
Buffer::isEmpty(void)
{
	return _buffer.empty();
}

void
Buffer::dumpData(char *dump, int size)
{
	for (int i = 0; i < size; i++)
		dump[i] = _buffer[i];
}

size_t
Buffer::size(void)
{
	return _buffer.size();
}
