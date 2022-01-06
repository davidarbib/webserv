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
# include "Connection.hpp"
# include "typedefs.hpp"

# define DELAY		1
# define BUFSIZE	2000
# define PORT		8003
# define IP			"127.0.0.1"
# define HOSTNAME	"w3bs0rv.com"

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
		Server(std::string, std::string, uint16_t, std::string, std::string);
		virtual ~Server(void);

		std::map<fd_t, RequestHandler*>	&getRefRequestHandlers();
		std::map<fd_t, RequestHandler*>	getRequestHandlers() const;

		fd_t				listenSocket(void);
		bool				isThereConnectionRequest(void);
		void				createConnection(void);
		void				watchInput(void);

		static void			setFdset(void);
		static void			initFdset(void);

		static fd_t			max_fd;
		static fd_set		read_fds;
		static fd_set 		write_fds;
		static fd_set 		origin_fds;

	private:
		std::string						_name;
		std::string						_ip;
		uint16_t						_port;
		std::string 					_access_logs_path;
		std::string 					_error_logs_path;
		fd_t							_listen_fd;
		std::map<fd_t, Connection*>		_connections;
		std::map<fd_t, RequestHandler*>	_request_handlers;

		void				transferToBuffer(fd_t connection_fd, char *buf);
		void				recvSend(void);
		bool				isThereSomethingToRead(fd_t);
		void				addWatchedFd(fd_t);
		void				delWatchedFd(fd_t);

		Server(void);
		Server(Server const &src);
};

#endif
