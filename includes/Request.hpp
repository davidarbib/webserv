#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <cctype>
#include "AHttpMessage.hpp"
#include "Response.hpp"

#define METHOD_NB 4

class Request : public AHttpMessage
{
	private:

		std::string 	_method_list[METHOD_NB];
		request_line 	_start_line;
		Response 		_response;
		bool			_start_line_initialized;
		bool			_headers_initialized;
		bool			_request_finalized;

		void
		initMethodList(void);

		bool
		isAllowedMethod(std::string const &method) const;

		bool
		isValidMethod(std::string const &method) const;

	public:

		Request(void);
		Request(Request &cpy);
		Request &
		operator=(Request const &src);
		virtual ~Request(void);

		void
		setMethodToken(std::string const &method_token);

		void
		setRequestURI(std::string const &request_URI);

		void
		setHttpVersion(std::string const &http_version);

		void
		setBody(std::string const& body);

		void
		setHeader(std::string const &key, std::string const &value);

		void
		printMessage(std::ostream &flux) const;

		void
		setStartLineInitialized(bool value);

		void
		setHeaderInitialized(bool value);

		void
		setRequestFinalized(bool value);

		bool
		iStartLineInitialized(void) const;

		bool
		isHeadersInitialized(void) const;

		bool
		isRequestFinalized(void) const;

		bool
		has_body(void) const;

		bool
		hadOctetInBody(char c);

		std::string
		get_header_value(std::string const &header_name) const;

		const request_line
		getStartLine(void) const;

		Response
		get_response(void);
};

std::ostream &
operator<<(std::ostream &flux, Request const &request);

#endif
