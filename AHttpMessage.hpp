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

		hash_map 	_headers;
		char		*_body;

	public:

		virtual void
		set_body(char *body) = 0;

		virtual void
		set_header(std::string const &key, std::string const &value) = 0;

		void
		print_message(std::ostream &flux) const;

		std::string const
		get_date(void) const;

};

#endif