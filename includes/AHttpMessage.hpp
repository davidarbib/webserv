#ifndef AHTTP_MESSAGE_HPP
#define AHTTP_MESSAGE_HPP

#include <iostream>
#include <string>
#include <map>
#include <ctime>

#define DATE_BUFFER 80

struct request_line
{
	std::string method_token;
	std::string request_URI;
	std::string http_version;
};

struct status_line
{
	std::string protocol_version;
	int 		status_code;
	std::string	reason_phrase;
};

class AHttpMessage
{
	protected:

		typedef std::map<std::string, std::string> hash_map;

		hash_map 		_headers;
		std::string		_body;

	public :

		virtual void
		setBody(std::string const& body) = 0;

		virtual void
		setHeader(std::string const &key, std::string const &value) = 0;

		std::string const&
		getBody(void) const;

		void
		printMessage(std::ostream &flux) const;

		std::string const
		getDate(void) const;

		std::string
		get_header_value(std::string const &header_name) const;

};

#endif