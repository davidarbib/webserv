#ifndef PARSING_TOOLS_HPP
#define PARSING_TOOLS_HPP

#include <string>
#include "Response.hpp"
#include <vector>
#define HEADER_SEP ":"

bool
isEndLine(AHttpMessage::body_type &line, int index);

bool
isEndSection(AHttpMessage::body_type &line, int index);

int
getHeader(int index, std::string &src, Response &response);

bool
isItEndLine(AHttpMessage::body_type::const_iterator it);

bool
isItEndSection(AHttpMessage::body_type::const_iterator it);

#endif
