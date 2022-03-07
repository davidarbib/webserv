#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

class Request;

# include "typedefs.hpp"
# include <string>
# include <iostream>
# include <list>
# include "Request.hpp"
# include "Connection.hpp"

typedef std::list<Request>::iterator RequestIt;

class RequestHandler
{
	private:

		RequestIt		_request_it;
		Connection		&_connection;

		RequestHandler
		&operator=(RequestHandler const &rhs);

		RequestHandler(void);

	public:

		RequestHandler(RequestIt request_it, Connection *connection);

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

		RequestIt
		getRequest(void) const;

		void
		clearBuffer(void);

		void
		clearRequest(void);
};

#endif
