#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <cctype>
#include "AHttpMessage.hpp"
#include "Response.hpp"

class Request : public AHttpMessage
{
	private:

		request_line 	_start_line;
		bool			_start_line_initialized;
		bool			_headers_initialized;
		bool			_request_finalized;
		bool			_valid;

	public:

		Request(void);
		Request(Request &cpy);
		Request &
		operator=(Request const &src);
		virtual ~Request(void);

		bool
		getValid(void) const;

		void
		setValid(bool value);

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
        isContentLengthCorrect(void) const;

		bool
		has_body(void) const;

		bool
		hadOctetInBody(char c);

		const request_line
		getStartLine(void) const;
};

std::ostream &
operator<<(std::ostream &flux, Request const &request);

#endif
