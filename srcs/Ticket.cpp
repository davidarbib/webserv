#include "Ticket.hpp"

Ticket::Ticket(Connection &connection, Request *request, Server const &server)
: _connection(connection), _request(request), _server(server)
{
}

Ticket::Ticket(Ticket const &src)
: _connection(src._connection), _request(src._request), _server(src._server)
{
}

Ticket::~Ticket(void)
{
}

Connection &
Ticket::getConnection(void)
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
