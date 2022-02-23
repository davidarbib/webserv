#include "Ticket.hpp"

Ticket::Ticket(Connection &connection, Request *request, Server const &server,
				std::map<fd_t, RequestHandler>::iterator rh_it)
: _connection(connection), _request(request), _server(server), _rh_it(rh_it)
{
}

Ticket::Ticket(Ticket const &src)
: _connection(src._connection), _request(src._request), _server(src._server),
	_rh_it(src._rh_it)
{
}

Ticket::~Ticket(void)
{
}

Connection &
Ticket::getConnection(void) const
{
	return _connection;
}

Request &
Ticket::getRequest(void)
{
	return *_request;
}

Server const &
Ticket::getServer(void) const
{
	return _server;
}

Ticket::rh_iterator
Ticket::getRhIt(void) const
{
	return _rh_it;
}
