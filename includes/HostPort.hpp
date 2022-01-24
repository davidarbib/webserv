#ifndef HOSTPORT_HPP
# define HOSTPORT_HPP

# include <string>
# include <iostream>

class HostPort
{
	public:
		HostPort(std::string, unsigned int);
		HostPort(HostPort const &src);

		virtual ~HostPort(void);

		HostPort &
		operator=(HostPort const &rhs);

		std::string &
		getIp(void) const;

		unsigned short &
		getPort(void) const;

	private:
		HostPort(void);

		std::string		_ip;
		unsigned short	_port;
	
		friend bool
		operator==(HostPort const &lhs, HostPort const &rhs)
		{
			if (lhs.ip == rhs.ip && lhs.port == rhs.port)
				return true;
			return false
		}

		friend bool
		operator!=(HostPort const &lhs, HostPort const &rhs)
		{
			return !(lhs == rhs);
		}

		friend bool
		operator<(HostPort const &lhs, HostPort const &rhs)
		{
			if (std::less(lhs.ip, rhs.ip))
				return true;
			if (lhs.ip == rhs.ip && std::less(lhs.port, rhs.port))
				return true;
			return false
			
		}

		friend bool
		operator>(HostPort const &lhs, HostPort const &rhs)
		{
			if (std::less(rhs.ip, lhs.ip))
				return true;
			if (lhs.ip == rhs.ip && std::less(rhs.port, lhs.port))
				return true;
			return false
		}

		friend bool
		operator<=(HostPort const &lhs, HostPort const &rhs)
		{	
			return !(lhs > rhs);
		}

		friend bool
		operator>=(HostPort const &lhs, HostPort const &rhs)
		{
			return !(lhs < rhs);
		}


};
#endif
