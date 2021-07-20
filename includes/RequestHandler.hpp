#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include "Request.hpp"

class RequestHandler
{
	public:

		enum	e_request_state
		{
			start,
			header,
			body,
			end
		};

	private:

		int						_idx;
		std::string				_in_buffer;
		enum e_request_state	_state;
		Request					*_request;

		RequestHandler
		&operator=(RequestHandler const &rhs);

		bool
		isEndLine(std::string &line, int index);

	public:

		RequestHandler(void);

		RequestHandler(RequestHandler const &src);

		virtual	~RequestHandler(void);

		void
		setIdx(int);

		void
		incIdx(int);

		int
		getIdx(void) const;

		void
		fillBuffer(char *raw_buffer);

		std::string &
		getBuffer(void);

		Request *
		getRequest(void) const;

		void
		clearBuffer(void);
};

#endif
