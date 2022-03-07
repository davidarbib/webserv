#include "Ticket.hpp"

//Ticket::Ticket(Connection &connection, Request *request, Server const &server,
//				std::map<fd_t, RequestHandler>::iterator rh_it)
//: _connection(connection), _request(request), _server(server), _rh_it(rh_it)
//{
//}
//

Ticket::Ticket(Connection &connection, RequestIt request_it, Server const &server)
: _connection(connection), _request_it(request_it), _server(server)
{
}

//Ticket::Ticket(Ticket const &src)
//: _connection(src._connection), _request(src._request), _server(src._server),
//	_rh_it(src._rh_it)
//{
//}

Ticket::Ticket(Ticket const &src)
: _connection(src._connection), _request_it(src._request_it), _server(src._server)
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

RequestIt
Ticket::getRequest(void) const
{
	return _request_it;
}

Server const &
Ticket::getServer(void) const
{
	return _server;
}

//Ticket::rh_iterator
//Ticket::getRhIt(void) const
//{
//	return _rh_it;
//}

void
Ticket::clearRequest(void)
{
}
