#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "AHttpMessage.hpp"

class Response : public AHttpMessage
{
	private:

		status_line	start_line;
};

#endif