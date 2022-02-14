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