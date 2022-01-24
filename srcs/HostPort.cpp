#include "HostPort.hpp"

HostPort::HostPort(std::string ip, unsigned int port)
: _ip(ip), _port(port)
{
}

HostPort::HostPort(HostPort const &src)
: _ip(src.ip), _port(src.port)
{
}

HostPort::~HostPort(void)
{
}

HostPort &
HostPort::operator=(HostPort const &rhs)
{
	this->_ip = rhs._ip;
	this->_port = rhs._port;
	return *this;
}

std::string &
HostPort::getIp(void) const
{
	return _ip;
}

unsigned short &
HostPort::getPort(void) const
{
	return _port;
}
