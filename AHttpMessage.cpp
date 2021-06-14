#include "AHttpMessage.hpp"

void
AHttpMessage::print_message(std::ostream &flux) const
{
	hash_map::const_iterator it;
	flux << "---------------------" << "Headers :" << "---------------------" << std::endl;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		flux << it->first << ": " << it->second << std::endl;
	}
	flux << "---------------------" << "Body :" << "---------------------" << std::endl;
	if (this->_body)
	{
		for (int i = 0; this->_body[i]; i++)
			flux << this->_body[i];
	}
}