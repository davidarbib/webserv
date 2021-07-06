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
		std::string				&_in_buffer;
		enum e_request_state	_state;
		Request					*_request;

		RequestHandler(void);

		RequestHandler
		&operator=(RequestHandler const &rhs);

	public:

		RequestHandler(std::string &);

		RequestHandler(RequestHandler const &src);

		virtual	~RequestHandler(void);
};

#endif
