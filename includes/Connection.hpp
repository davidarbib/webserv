#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <iostream>
# include "typedefs.hpp"
# include <sstream>

class Server;

class Connection
{
	public:
		Connection(fd_t, unsigned long, unsigned short, Server &server);
		Connection	&operator=(Connection const &rhs);
		Connection(Connection const &src);
		~Connection(void);

		/*
		** getters return ip and port in host format
		** or string form
		*/

		fd_t
		getSocketFd(void) const;

		unsigned long
		getClientIp(void) const;

		std::string
		getClientIpStr(void) const;

		unsigned short
		getClientPort(void) const;

		std::string
		getClientPortStr(void) const;

		Server&
		getServer(void) const;

	private:
		/*
		** ! _client_ip and _client_port are in network format
		*/
		fd_t			_socket_fd;
		unsigned long	_client_ip;
		unsigned short	_client_port;
		std::string		_client_ip_str;
		std::string		_client_port_str;
		Server			&_server;

		Connection(void);

		void
		makeIpStr(void);

		void
		makePortStr(void);

};

#include "Server.hpp"

#endif
