#ifndef AHTTP_MESSAGE_HPP
#define AHTTP_MESSAGE_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
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
	public:
		typedef std::vector<char> body_type;
		typedef std::map<std::string, std::string> hash_map;
	
	protected:
		hash_map 		_headers;
		body_type		_body;

	public :
		void
		setBody(AHttpMessage::body_type const& body);

		virtual void
		setHeader(std::string const &key, std::string const &value) = 0;

		body_type const&
		getBody(void) const;

		void
		printMessage(std::ostream &flux) const;

		std::string const
		getDate(void) const;

		std::string
		getHeaderValue(std::string const &header_name) const;

};

#endif
