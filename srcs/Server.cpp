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

void
Server::listen()
{
	sockaddr_in sin;

	this->_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_fd == -1)
		throw ListenException();
	if (fcntl(this->_listen_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ListenException();
	maxfd = sock_fd;


	memset(reinterpret_cast<void*>(&sin), 0, sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);

	if (bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == -1)
		throw ListenException();
	listen(sock_fd, 1);
}

bool				
Server::theresConnectionRequest()
{
	if (FD_ISSET(sock_fd, &read_fds))
		return 1;
	return 0;
}

void
Server::addWatchedFd(int fd)
{
	FD_SET(fd, &this->_origin_fds);
	if (fd > this->_maxfd)
		this->_maxfd = fd;
}

void
Server::delWatchedFd(int fd)
{
	FD_CLR(fd, &this->_origin_fds);
}

void
Server::createConnection()
{
	sockaddr_in new_sin;
	int			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

	std::cout << "connection" << std::endl;
	new_sock_fd = accept(this->_listen_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	if (new_sock_fd < 0)
		throw ConnectionException();
	if (fcntl(new_sock_fd, F_SETFL, O_NONBLOCK) < 0)
		throw ConnectionException();
	addWatchedFd(new_sock_fd);
	this->_connections_fd.push_back(new_sock_fd);
	FD_CLR(sock_fd, &read_fds);
}

void
Server::watchInput()
{
	char buf[BUFSIZE];
	bzero(buf, BUFSIZE);

	std::vector<int::iterator fd_ptr;
	for (fd_ptr = this->_connections_fd.begin(); fd_ptr != this->_connections_fd.end(); fd_ptr++)
	{
		if (!theresSomethingToRead(*fd_ptr))
			continue ;
		int recvret = recv(*fd_ptr, buf, BUFSIZE, NULL);	
		if (!recvret)
		{
			delWatchedFd(*fd_ptr);
			close(*fd_ptr);	
			this->_connections_fd.erase(fd_ptr);
			continue ;
		}
		printf("%s\n", buf);
		FD_CLR(*fd_ptr, &this->_read_fds);
	}
}

bool
theresSomethingToRead(int fd)
{
	if (FD_ISSET(fd, &this->_read_fds))
		return 1;
	return 0;
}

void
Server::recvSend()
{
	struct timeval tv;

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	FD_ZERO(&this->_origin_fds);
	FD_SET(sock_fd, &this->_origin_fds);

	while (1)
	{
		this->_read_fds = this->_origin_fds; 
		this->_write_fds = this->_origin_fds; 
		select(maxfd + 1, &this->_read_fds, &this->_write_fds, NULL, &tv);
		if (theresConnectionRequest())
			createConnection();	
		watchInput();
		if (FD_ISSET(fds[0], &write_fds) && !writeyet)
		{
			printf("write in socket : %d\n", fds[0]);
			write(fds[0], "coucou\n", 7);
			FD_CLR(fds[0], &write_fds);
			writeyet = 1;
		}
	}
}
