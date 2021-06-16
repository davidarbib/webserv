#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>

# define DELAY	5


class Server
{
	public:
		Server(void);
		Server(Server const &src);
		virtual ~Server(void);
		Server	&operator=(Server const &rhs);

	private:
		fd_set				_read_fds;
		fd_set 				_write_fds;
		fd_set 				_origin_fds;
		int					_listen_port;
		int					_listen_fd;
		int					_maxfd;
		std::string			_name;
		std::string 		_access_logs_path;
		std::string 		_error_logs_path;
		std::vector<int>	_connections_fd;

		void				listen();
};

#endif
