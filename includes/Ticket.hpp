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
	//typedef std::map<fd_t, RequestHandler>::iterator	rh_iterator;

	public:
		//Ticket(Connection &connection, Request *request, Server const &server,
		//		std::map<fd_t, RequestHandler>::iterator rh_it);
		Ticket(Connection &connection, RequestIt request_it, Server const &server);

		virtual	~Ticket(void);
		
		Ticket(Ticket const &src);

		Connection &
		getConnection(void) const;
		
		RequestIt
		getRequest(void) const;
		
		Server const &
		getServer(void) const;

		//rh_iterator
		//getRhIt(void) const;

		void
		clearRequest(void);

	private:
		Connection		&_connection;
		RequestIt		_request_it;
		Server const 	&_server;

		Ticket(void);
};
#endif
