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
		Ticket(Connection const &connection, Request *request, Server const &server);
		virtual	~Ticket(void);
		
		Connection const &
		getConnection(void) const;
		
		Request const &
		getRequest(void) const;
		
		Server const &
		getServer(void) const;

	private:
		Connection const 	&_connection;
		Request				*_request;
		Server const 		&_server;

		Ticket(void);
		Ticket(Ticket const &src);
		Ticket	&operator=(Ticket const &rhs);
};
#endif
