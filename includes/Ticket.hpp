#ifndef TICKET_HPP
# define TICKET_HPP

# include <string>
# include <iostream>
# include "Connection.hpp"
# include "Request.hpp"
# include "RequestHandler.hpp"
# include "Server.hpp"

class Ticket
{
	typedef std::map<fd_t, RequestHandler>::iterator	rh_iterator;

	public:
		Ticket(Connection &connection, Request *request, Server const &server,
				std::map<fd_t, RequestHandler>::iterator rh_it);
		virtual	~Ticket(void);
		
		Ticket(Ticket const &src);
		
		Connection &
		getConnection(void) const;
		
		Request &
		getRequest(void);
		
		Server const &
		getServer(void) const;

		rh_iterator
		getRhIt(void) const;

		Ticket	&operator=(Ticket const &rhs);

	private:
		Connection		&_connection;
		Request			*_request;
		Server const 	&_server;
		rh_iterator		_rh_it;

		Ticket(void);
};
#endif
