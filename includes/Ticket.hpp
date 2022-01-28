#ifndef TICKET_HPP
# define TICKET_HPP

# include <string>
# include <iostream>
# include "Connection.hpp"
# include "Request.hpp"
# include "Server.hpp"

class Ticket
{
	public:
		Ticket(Connection &connection, Request *request, Server const &server);
		virtual	~Ticket(void);
		
		Ticket(Ticket const &src);
		
		Connection &
		getConnection(void);
		
		Request const &
		getRequest(void) const;
		
		Server const &
		getServer(void) const;

		Ticket	&operator=(Ticket const &rhs);

	private:
		Connection			&_connection;
		Request				*_request;
		Server const 		&_server;

		Ticket(void);
};
#endif
