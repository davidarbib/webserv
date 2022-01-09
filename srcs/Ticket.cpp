#include "Ticket.hpp"

Ticket::Ticket(Connection const &connection, Request *request, Server const &server)
: _connection(connection), _request(request), _server(server)
{
}

Ticket::~Ticket(void)
{
}

Connection const &
Ticket::getConnection(void) const
{
	return _connection;
}

Request const &
Ticket::getRequest(void) const
{
	return *_request;
}

Server const &
Ticket::getServer(void) const
{
	return _server;
}
