#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <iostream>
# include "typedefs.hpp"
# include <sstream>
# include <arpa/inet.h>
# include "Buffer.hpp"

class Connection
{
	public:
		Connection(fd_t, unsigned long, unsigned short);
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
		
		Buffer &
		getInBuffer(void);

		Buffer &
		getOutBuffer(void);

		void
		dumpOutBufferData(char *dump, int size);
		
		void
		fillBuffer(char *buf, int size);

		void
		eatOutBufferData(int);

	private:
		Connection(Connection const &src);
		Connection	&
		operator=(Connection const &rhs);

		/*
		** ! _client_ip and _client_port are in network format
		*/
		fd_t			_socket_fd;
		unsigned long	_client_ip;
		unsigned short	_client_port;
		std::string		_client_ip_str;
		std::string		_client_port_str;
		Buffer			_in_buffer;
		Buffer			_out_buffer;

		Connection(void);

		void
		makeIpStr(void);

		void
		makePortStr(void);

		friend Connection &
		operator<<(Connection &connection, std::string const & message);

		friend Connection &
		operator<<(Connection &connection,
				std::vector<char> const & message);

		friend bool
		operator<(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd < rhs._socket_fd; }

		friend bool
		operator<=(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd <= rhs._socket_fd; }

		friend bool
		operator>(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd > rhs._socket_fd; }

		friend bool
		operator>=(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd >= rhs._socket_fd; }

		friend bool
		operator==(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd == rhs._socket_fd; }

		friend bool
		operator!=(Connection const &lhs, Connection const &rhs)
		{ return lhs._socket_fd != rhs._socket_fd; }
};

#endif
