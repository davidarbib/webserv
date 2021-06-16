#include "Server.hpp"

Server::Server(void):
{
}

Server::Server(std::string name, int port, std::string _access_logs_path, std::string _error_logs_path)
:	_name(name),
	_port(port),
	_access_logs_path(access_logs_path),
	_error_logs_path(error_logs_path)
{
}

Server::Server(Server const &src)
{
}

Server::~Server(void)
{
}

Server	&Server::operator=(Server const &rhs)
{
}

void				Server::listen()
{
	sockaddr_in sin;
	struct timeval tv;

	this->_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(this->_listen_fd, F_SETFL, O_NONBLOCK);
	std::cout << "sock fd at beginning : " << sock_fd << std::endl;
	maxfd = sock_fd;

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	bzero(reinterpret_cast<void*>(&sin), sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);

	int retbind = bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
	if (retbind < 0)
	{
		perror("webserv");
		exit(EXIT_FAILURE);
	}
	listen(sock_fd, 1);
}
