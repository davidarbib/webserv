#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <iostream>
# include "typedefs.hpp"

class Server;

class Connection
{
	public:
		Connection(fd_t, unsigned long, unsigned short, Server &server);
		Connection	&operator=(Connection const &rhs);
		Connection(Connection const &src);
		~Connection(void);

		fd_t			getSocketFd(void) const;
		unsigned long	getClientIp(void) const;
		std::string		getClientIpStr(void) const;
		unsigned short	getClientPort(void) const;
		std::string		getClientPortStr(void) const;
		Server			&getServer(void) const;

	private:
		fd_t			_socket_fd;
		unsigned long	_client_ip;
		unsigned short	_client_port;
		Server			&_server;
		Connection(void);

};

#include "Server.hpp"

#endif
