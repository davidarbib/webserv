#include "Server.hpp"

Server::Server(std::string ip, std::string port,
				std::vector<ConfigServer> candidate_confs)
: _ip(ip), _port(port), _candidate_confs(candidate_confs)
{
	//int port_nb = std::atoi(_port.c_str());
	//_port_nb = static_cast<uint16_t>(port_nb);
	std::stringstream stream;
	stream << _port;
	stream >> _port_nb;
}

Server::Server(Server const &src)
: _ip(src._ip), _port(src._port), _port_nb(src._port_nb),
	_candidate_confs(src._candidate_confs)
{
}

Server::~Server(void)
{
}

std::string
Server::getPort(void)
{
	return _port;
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

std::vector<ConfigServer> const&
Server::getCandidateConfs(void) const
{
	return _candidate_confs;
}

void
Server::clearConnections(void)
{
    for (size_t i = 0; i < _connections.size(); i++)
        delete _connections[i];
}

fd_t
Server::listenSocket()
{
	sockaddr_in sin;

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == -1)
		throw ListenException();
	if (fcntl(_listen_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ListenException();
	int enable = 1;
	if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw ListenException();
	if (_listen_fd > Server::max_fd)
		Server::max_fd = _listen_fd;

	memset(reinterpret_cast<void*>(&sin), 0, sizeof(sockaddr_in));
	sin.sin_addr.s_addr = inet_addr(_ip.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_port_nb);

	if (bind(_listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == -1)
		throw ListenException();
	listen(_listen_fd, QUEUE_LIMIT);
	addWatchedFd(_listen_fd);
	std::cout << "listening on port : " << _port_nb << std::endl;
	return _listen_fd;
}

bool
Server::isThereConnectionRequest(void)
{
	if (FD_ISSET(_listen_fd, &Server::read_fds))
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

void
Server::createConnection(void)
{
	sockaddr_in		new_sin;
	fd_t			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

	new_sock_fd = accept(_listen_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
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
Server::transferToBuffer(fd_t connection_fd, char *buf, int size)
{
	_connections[connection_fd]->fillBuffer(buf, size);
}

void
Server::watchInput(std::map<fd_t, RequestHandler> &request_handlers)
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
		if (recvret <= 0)
		{
			delWatchedFd(connection_it->first);
			close(connection_it->first);
			std::map<fd_t, RequestHandler>::iterator rh_to_del =
				request_handlers.find(connection_it->second->getSocketFd());
			if (rh_to_del != request_handlers.end())
			{
				rh_to_del->second.clearRequest();
				request_handlers.erase(rh_to_del);
			}
			delete connection_it->second;
			_connections.erase(connection_it);
			connection_it = _connections.begin();
		}
		else
		{
			transferToBuffer(connection_it->first, buf, recvret);
			FD_CLR(connection_it->first, &Server::read_fds);
			connection_it++;
		}
	}
}

void
Server::send()
{
	std::map<fd_t, Connection*>::iterator connection_it;
	connection_it = _connections.begin();
	for (; connection_it != _connections.end(); connection_it++)
	{
		if (connection_it->second->getOutBuffer().size() == 0)
			continue;
		fd_t fd = connection_it->second->getSocketFd();
		if (isPossibleToWrite(fd))
		{
			char buf[BUFSIZE];
			bzero(buf, BUFSIZE);
			size_t bufsize = BUFSIZE;
			size_t write_size = std::min(bufsize - 1,
									connection_it->second->getOutBuffer().size());
			connection_it->second->dumpOutBufferData(buf, write_size);
			write(fd, buf, write_size); 
			connection_it->second->eatOutBufferData(write_size);
		}
	}
}

bool
Server::isThereSomethingToRead(fd_t fd)
{
	if (FD_ISSET(fd, &Server::read_fds))
		return true;
	return false;
}

bool
Server::isPossibleToWrite(fd_t fd)
{
	if (FD_ISSET(fd, &Server::write_fds))
		return true;
	return false;
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
