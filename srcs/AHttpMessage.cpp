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
		for (size_t i = 0; i < this->_body.size(); i++)
			flux << this->_body[i];
	}
}

AHttpMessage::body_type const&
AHttpMessage::getBody(void) const
{
	return this->_body;
}

std::string const
AHttpMessage::getDate(void) const
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

std::string
AHttpMessage::getHeaderValue(std::string const &header_name) const
{
	hash_map::const_iterator it;

	it = this->_headers.find(header_name);
	if (it != this->_headers.end())
		return it->second;
	return std::string();
}

void
AHttpMessage::setBody(AHttpMessage::body_type const& body)
{
	this->_body.insert(_body.end(), body.begin(), body.end());
}
