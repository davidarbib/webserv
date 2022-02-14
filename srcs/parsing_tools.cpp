#include "parsing_tools.hpp"

bool
isEndLine(std::string &line, int index)
{
	if (line[index] == '\r')
	{
		if (line[index + 1] == '\n')
			return true;
	}
	return false;
}

bool
isEndSection(std::string &line, int index)
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
	value = src.substr(sep_pos, crlf_pos - sep_pos);

	response.setHeader(key, value);
	return crlf_pos + 2;
}