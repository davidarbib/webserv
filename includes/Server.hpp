#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <map>
# include "RequestHandler.hpp"
# include "typedefs.hpp"

# define DELAY		1
# define BUFSIZE	2000
# define PORT		8003

typedef long fd_t;

class Server
{
	class ListenException : public std::exception
	{
		public :
			virtual const char*
			what() const
			throw()
			{ return "Error while listening socket creation\n"; }
	};

	class ConnectionException : public std::exception
	{
		public :
			virtual const char*
			what() const
			throw()
			{ return "Error while connection creation\n"; }
	};

	public:
		Server(std::string, int, std::string, std::string);
		virtual ~Server(void);

		std::map<fd_t, RequestHandler*>	&getRefRequestHandlers();
		std::map<fd_t, RequestHandler*>	getRequestHandlers() const;

		fd_t				listenSocket()
							throw(Server::ListenException);
		bool				isThereConnectionRequest();
		void				createConnection();
		void				watchInput();

		static void			setFdset();
		static void			initFdset();

		static fd_t			max_fd;
		static fd_set		read_fds;
		static fd_set 		write_fds;
		static fd_set 		origin_fds;

	private:
		std::string						_name;
		int								_port;
		std::string 					_access_logs_path;
		std::string 					_error_logs_path;
		fd_t							_listen_fd;
		std::vector<fd_t>				_connections_fd;
		std::map<fd_t, RequestHandler*>	_request_handlers;

		void				transferToBuffer(fd_t connection_fd, char *buf);
		void				recvSend();
		bool				isThereSomethingToRead(fd_t);
		void				addWatchedFd(fd_t);
		void				delWatchedFd(fd_t);
		void				delConnection(void);

		Server(void);
		Server(Server const &src);
		Server				&operator=(Server const &rhs);
};

#endif
