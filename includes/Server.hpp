#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <cstring>

# define DELAY		5
# define BUFSIZE	2000
# define PORT		8003

typedef int fd_t;

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
		Server				&operator=(Server const &rhs);
		static void			setFdset();
		static void			initFdset();

		static fd_t			max_fd;
		static fd_set		read_fds;
		static fd_set 		write_fds;
		static fd_set 		origin_fds;

	private:
		int					_listen_port;
		fd_t				_listen_fd;
		std::string			_name;
		std::string 		_access_logs_path;
		std::string 		_error_logs_path;
		std::vector<int>	_connections_fd;

		void				listen()
							throw ListenException();

		void				recvSend();
		int					createConnection();
		bool				theresConnectionRequest();
		bool				theresSomethingToRead(fd_t);
		void				addWatchedFd(fd_t);
		void				delWatchedFd(fd_t);
		void				watchInput();
};

#endif
