#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <cstring>

# define DELAY	5
# define BUFSIZE 2000

class Server
{
	class ListenException : public std::exception
	{
		public :
			virtual const char*
			what() const
			{ return "Error while listening socket creation\n"; }
	}

	class ConnectionException : public std::exception
	{
		public :
			virtual const char*
			what() const
			{ return "Error while connection creation\n"; }
	}

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

		void				listen()
							throw ListenException();

		void				recvSend();
		int					createConnection();
		bool				theresConnectionRequest();
		bool				theresSomethingToRead(int);
		void				addWatchedFd(int);
		void				delWatchedFd(int);
		void				watchInput();
};

#endif
