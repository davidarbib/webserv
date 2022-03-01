#include "parsing_tools.hpp"

bool
isEndLine(AHttpMessage::body_type &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool
isEndSection(AHttpMessage::body_type &line, int index)
{
	if (isEndLine(line, index))
	{
		if (isEndLine(line, index + 2))
			return true;
	}
	return false;
}

int
getHeader(int index, std::string &src, Response &response)
{
	std::string key;
	std::string value;

	size_t sep_pos = src.find(HEADER_SEP, index);
	size_t crlf_pos = src.find(CRLF_str, index);

	key = src.substr(index, sep_pos - index);
	value = src.substr(sep_pos + 1, crlf_pos - sep_pos);
	value = value.substr(value.find_first_not_of(" "));
	response.setHeader(key, value);
	return crlf_pos + 2;
}

bool
isItEndLine(AHttpMessage::body_type::iterator it)
{
	if (it[0] == '\r' && it[1] == '\n')
		return true;
	return false;
}

bool
isItEndSection(AHttpMessage::body_type::iterator it)
{
	if (isItEndLine(it) && isItEndLine(it + 2))
		return true;
	return false;
}
