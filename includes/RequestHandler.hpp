#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include "Request.hpp"
# include "Connection.hpp"
# include "Buffer.hpp"

class RequestHandler
{
	private:

		int						_idx;
		Buffer					&_in_buffer;
		Request					*_request;
		Connection				*_connection;

		RequestHandler
		&operator=(RequestHandler const &rhs);

		RequestHandler(void);

	public:

		RequestHandler(Connection *connection, Buffer &buffer);

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

		static bool
		isEndLine(std::string &line, int index);

		void
		clearBuffer(int);
};

#endif
