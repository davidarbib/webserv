#ifndef HOSTPORT_HPP
# define HOSTPORT_HPP

# include <string>
# include <iostream>

class HostPort
{
	public:
		HostPort(std::string, std::string);
		HostPort(HostPort const &src);

		virtual ~HostPort(void);

		HostPort &
		operator=(HostPort const &rhs);

		std::string const &
		getIp(void) const;
		
		std::string const &
		getPort(void) const;

	private:
		HostPort(void);

		std::string		_ip;
		std::string		_port;
	
		friend bool
		operator==(HostPort const &lhs, HostPort const &rhs)
		{
			if (lhs._ip == rhs._ip && lhs._port == rhs._port)
				return true;
			return false;
		}

		friend bool
		operator!=(HostPort const &lhs, HostPort const &rhs)
		{
			return !(lhs == rhs);
		}

		friend bool
		operator<(HostPort const &lhs, HostPort const &rhs)
		{
			if (lhs._ip < rhs._ip)
				return true;
			if (lhs._ip == rhs._ip && lhs._port < rhs._port)
				return true;
			return false;
			
		}

		friend bool
		operator>(HostPort const &lhs, HostPort const &rhs)
		{
			if (lhs._ip < rhs._ip)
				return true;
			if (lhs._ip == rhs._ip && rhs._port < lhs._port)
				return true;
			return false;
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
