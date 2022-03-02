#include "Connection.hpp"

Connection::Connection(fd_t fd, unsigned long client_ip, unsigned short client_port)
: _socket_fd(fd), _client_ip(client_ip), _client_port(client_port)
{
	makeIpStr();
	makePortStr();
	_expect_body = false;
}

Connection::~Connection(void)
{ }

fd_t			
Connection::getSocketFd(void) const
{ return _socket_fd; }

unsigned long		
Connection::getClientIp(void) const
{ return ntohl(_client_ip); }

unsigned short
Connection::getClientPort(void) const
{ return ntohs(_client_port); }

std::string
Connection::getClientIpStr(void) const
{ return _client_ip_str; }

std::string
Connection::getClientPortStr(void) const
{ return _client_port_str; }

Buffer &
Connection::getInBuffer(void)
{
	return _in_buffer;
}

Buffer &
Connection::getOutBuffer(void)
{
	return _out_buffer;
}

void
Connection::dumpOutBufferData(char *dump, int size)
{
	_out_buffer.dumpData(dump, size);
}

void
Connection::fillBuffer(char *buf, int size)
{
	_in_buffer.fillBuffer(buf, size);	
}

void
Connection::eatOutBufferData(int size)
{
	_out_buffer.setIdx(size);
	_out_buffer.clearBuffer();
}

bool
Connection::isFullBodyExpected(void)
{
	return _expect_body;
}

void
Connection::expectFullBodyNextRequest(void)
{
	_expect_body = true;
}

void
Connection::makeIpStr(void)
{
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &_client_ip, ip, INET_ADDRSTRLEN);
	_client_ip_str = std::string(ip);
}

void
Connection::makePortStr(void)
{
	std::stringstream stream;
	stream << ntohs(_client_port);
	stream >> _client_port_str;
}

Connection &
operator<<(Connection &connection, std::string const & message)
{
	connection._out_buffer.append(message);
	return connection;
}

Connection &
operator<<(Connection &connection, std::vector<char> const & message)
{
	connection._out_buffer.append(message);
	return connection;
}
