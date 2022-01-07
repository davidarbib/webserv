#include "AHttpMessage.hpp"

void
AHttpMessage::printMessage(std::ostream &flux) const
{
	hash_map::const_iterator it;
	flux << "---------------------" << "Headers :" << "---------------------" << std::endl;
	for (it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		flux << it->first << ": " << it->second << std::endl;
	}
	flux << "---------------------" << "Body :" << "---------------------" << std::endl;
	if (!this->_body.empty())
	{
		for (int i = 0; this->_body[i]; i++)
			flux << this->_body[i];
	}
}

std::string &
AHttpMessage::get_body(void)
{
	return this->_body;
}

std::string const
AHttpMessage::get_date(void) const
{
	time_t 		raw_time;
	struct 		tm *ptm;
	char		buffer[DATE_BUFFER];
	std::string date;

	time(&raw_time);
	ptm = gmtime(&raw_time);
	std::strftime(buffer, DATE_BUFFER ,"%a, %d %b %g %T GMT", ptm);
	date = buffer;
	return date;
}