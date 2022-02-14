#ifndef PARSING_TOOLS_HPP
#define PARSING_TOOLS_HPP

#include <string>
#include "Response.hpp"
#define HEADER_SEP ":"

bool
isEndLine(std::string &line, int index);

bool
isEndSection(std::string &line, int index);

int
getHeader(int index, std::string &src, Response &response);

#endif