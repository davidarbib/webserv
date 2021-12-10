#include "Connection.hpp"

Connection::Connection(fd_t fd, unsigned long client_ip, unsigned short client_port,
						Server &server)
: _socket_fd(fd), _client_ip(client_ip), _client_port(client_port),
	_server(server)
{ }

Connection::Connection(Connection const &src)
: _socket_fd(src._socket_fd), _client_ip(src._client_ip),
	_client_port(src._client_port), _server(src._server)
{ }

Connection::~Connection(void)
{ }

Connection&
Connection::operator=(Connection const &rhs)
{
	_socket_fd = rhs._socket_fd;
	_client_ip = rhs._client_ip;
	_client_port = rhs._client_port;
	_server = rhs._server;
	return *this;
}

fd_t			
Connection::getSocketFd(void) const
{ return _socket_fd; }

unsigned long		
Connection::getClientIp(void) const
{ return _client_ip; }

unsigned short
Connection::getClientPort(void) const
{ return _client_port; }

Server &
Connection::getServer(void) const
{ return _server; }
