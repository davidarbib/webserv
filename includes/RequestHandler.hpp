#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

class Request;

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include "Request.hpp"
# include "Connection.hpp"

class RequestHandler
{
	private:

		Request					*_request;
		Connection				&_connection;

		RequestHandler
		&operator=(RequestHandler const &rhs);

		RequestHandler(void);

	public:

		RequestHandler(Request *request, Connection *connection);

		RequestHandler(RequestHandler const &src);

		virtual	~RequestHandler(void);

		void
		setIdx(int);

		void
		incIdx(int);

		int
		getIdx(void) const;

		void
		fillBuffer(char *raw_buffer, int size);

		std::vector<char> &
		getBuffer(void);

		Request *
		getRequest(void) const;

		void
		clearBuffer(void);
};

#endif
