#include "HostPort.hpp"

HostPort::HostPort(void)
{
}

HostPort::HostPort(std::string ip, std::string port)
: _ip(ip), _port(port)
{
}

HostPort::HostPort(HostPort const &src)
: _ip(src._ip), _port(src._port)
{
}

HostPort::~HostPort(void)
{
}

HostPort &
HostPort::operator=(HostPort const &rhs)
{
	this->_ip = rhs._ip;
	std::cout << "ip is affected" << std::endl;
	this->_port = rhs._port;
	std::cout << "port is affected" << std::endl;
	return *this;
}

std::string const &
HostPort::getIp(void) const
{
	return _ip;
}

std::string const &
HostPort::getPort(void) const
{
	return _port;
}
