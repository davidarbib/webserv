#include "Server.hpp"

Server::Server(std::string name, std::string ip, unsigned short port, std::string access_logs_path, std::string error_logs_path)
:	_name(name),
	_ip(ip),
	_port(port),
	_access_logs_path(access_logs_path),
	_error_logs_path(error_logs_path)
{
}

Server::~Server(void)
{
}

std::map<fd_t, Connection*> &
Server::getRefConnections(void)
{
	return _connections;
}

std::map<fd_t, Connection*>
Server::getConnections(void) const
{
	return _connections;
}

fd_t
Server::listenSocket()
{
	sockaddr_in sin;

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == -1)
		throw ListenException();
	int enable = 1;		
	if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw ListenException();
	if (fcntl(_listen_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ListenException();
	if (_listen_fd > Server::max_fd)
		Server::max_fd = _listen_fd;

	memset(reinterpret_cast<void*>(&sin), 0, sizeof(sockaddr_in));
	sin.sin_addr.s_addr = inet_addr(_ip.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_port);

	if (bind(_listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == -1)
		throw ListenException();
	listen(_listen_fd, 1);
	addWatchedFd(_listen_fd);
	return _listen_fd;
}

void
Server::createConnection(void)
{
	sockaddr_in		new_sin;
	fd_t			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

//	std::cout << "connection" << std::endl;
	new_sock_fd = accept(_listen_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
//	std::cout << "client port : " << (int)ntohs(new_sin.sin_port) << std::endl;
//	std::cout << "client IP : " << inet_ntoa(new_sin.sin_addr) << std::endl;
//	std::cout << "client IP : " << ((unsigned char *)&new_sin.sin_addr)[0] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[1] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[2] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[3] << std::endl;
	if (new_sock_fd < 0)
		throw ConnectionException();
	if (fcntl(new_sock_fd, F_SETFL, O_NONBLOCK) < 0)
		throw ConnectionException();
	addWatchedFd(new_sock_fd);
	_connections[new_sock_fd] = new Connection(new_sock_fd,
								static_cast<unsigned long>(new_sin.sin_addr.s_addr),
								static_cast<unsigned short>(new_sin.sin_port));
	FD_CLR(_listen_fd, &read_fds);
}

void
Server::transferToBuffer(fd_t connection_fd, char *buf)
{
	_connections[connection_fd]->fillBuffer(buf);
}

void
Server::watchInput()
{
	char buf[BUFSIZE];
	memset(reinterpret_cast<void*>(buf), 0, BUFSIZE);

	std::map<fd_t, Connection*>::iterator connection_it;
	connection_it = _connections.begin();
	while (connection_it != _connections.end())
	{
		if (!isThereSomethingToRead(connection_it->first))
		{
			connection_it++;
			continue ;
		}
		int recvret = recv(connection_it->first, buf, BUFSIZE, 0);
		if (recvret == 0)
		{
			delWatchedFd(connection_it->first);
			close(connection_it->first);
			_connections.erase(connection_it);
			connection_it = _connections.begin();
		}
		else
		{
			transferToBuffer(connection_it->first, buf);
			FD_CLR(connection_it->first, &Server::read_fds);
			connection_it++;
		}
	}
}

bool				
Server::isThereConnectionRequest(void)
{
	if (FD_ISSET(_listen_fd, &Server::read_fds))
		return 1;
	return 0;
}

bool
Server::isWritePossible(fd_t fd)
{
	if (FD_ISSET(fd, &Server::write_fds))
		return 1;
	return 0;
}

void 
Server::addWatchedFd(fd_t fd)
{
	FD_SET(fd, &Server::origin_fds);
	if (fd > Server::max_fd)
		Server::max_fd = fd;
}

void
Server::delWatchedFd(fd_t fd)
{
	FD_CLR(fd, &Server::origin_fds);
}

bool
Server::isThereSomethingToRead(fd_t fd)
{
	if (FD_ISSET(fd, &Server::read_fds))
		return 1;
	return 0;
}

void
Server::setFdset()
{
	Server::read_fds = Server::origin_fds; 
	Server::write_fds = Server::origin_fds; 
}

void
Server::initFdset()
{
	FD_ZERO(&Server::read_fds);
	FD_ZERO(&Server::write_fds);
	FD_ZERO(&Server::origin_fds);
}

fd_t		Server::max_fd = 0;

fd_set		Server::read_fds;
fd_set 		Server::write_fds;
fd_set 		Server::origin_fds;
