#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include "Request.hpp"
# include "Connection.hpp"

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
		Connection				*_connection;

		RequestHandler
		&operator=(RequestHandler const &rhs);

		RequestHandler(void);

	public:

		RequestHandler(Connection *connection);

		RequestHandler(RequestHandler const &src);

		virtual	~RequestHandler(void);

		void
		setIdx(int);

		void
		incIdx(int);

		void
		incState(void);

		int
		getIdx(void) const;

		void
		fillBuffer(char *raw_buffer);

		std::string &
		getBuffer(void);

		Request *
		getRequest(void) const;

		static bool
		isEndLine(std::string &line, int index);

		void
		clearBuffer(int);
};

#endif
