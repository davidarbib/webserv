#include "Server.hpp"

Server::Server(std::string name, int port, std::string access_logs_path, std::string error_logs_path)
:	_name(name),
	_port(port),
	_access_logs_path(access_logs_path),
	_error_logs_path(error_logs_path)
{
}

Server::~Server(void)
{
}

std::map<fd_t, RequestHandler*> &
Server::getRefRequestHandlers(void)
{
	return this->_request_handlers;
}

std::map<fd_t, RequestHandler*>
Server::getRequestHandlers(void) const
{
	return this->_request_handlers;
}

fd_t
Server::listenSocket()
throw(Server::ListenException)
{
	sockaddr_in sin;

	this->_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listen_fd == -1)
		throw ListenException();
	if (fcntl(this->_listen_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ListenException();
	if (this->_listen_fd > Server::max_fd)
		Server::max_fd = this->_listen_fd;

	memset(reinterpret_cast<void*>(&sin), 0, sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);

	if (bind(this->_listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == -1)
		throw ListenException();
	listen(this->_listen_fd, 1);
	addWatchedFd(this->_listen_fd);
	return this->_listen_fd;
}

bool				
Server::isThereConnectionRequest(void)
{
	if (FD_ISSET(this->_listen_fd, &Server::read_fds))
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
Server::delConnection(void)
{
}

void
Server::createConnection(void)
{
	sockaddr_in		new_sin;
	fd_t			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

	std::cout << "connection" << std::endl;
	new_sock_fd = accept(this->_listen_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	std::cout << "client port : " << (int)ntohs(new_sin.sin_port) << std::endl;
	std::cout << "client IP : " << inet_ntoa(new_sin.sin_addr) << std::endl;
//	std::cout << "client IP : " << ((unsigned char *)&new_sin.sin_addr)[0] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[1] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[2] << ".";
//	std::cout << (int)((unsigned char *)&new_sin.sin_addr)[3] << std::endl;
	if (new_sock_fd < 0)
		throw ConnectionException();
	if (fcntl(new_sock_fd, F_SETFL, O_NONBLOCK) < 0)
		throw ConnectionException();
	addWatchedFd(new_sock_fd);
	this->_connections_fd.push_back(new_sock_fd);
	// tmp fix
	std::string tmp = "";
	this->_request_handlers[new_sock_fd] = new RequestHandler(tmp);
	FD_CLR(this->_listen_fd, &read_fds);
}

void
Server::transferToBuffer(fd_t connection_fd, char *buf)
{
	this->_request_handlers[connection_fd]->fillBuffer(buf);
}

void
Server::watchInput()
{
	char buf[BUFSIZE];
	bzero(buf, BUFSIZE); //TODO

	std::vector<long>::iterator fd_ptr;
	fd_ptr = this->_connections_fd.begin();
	while (fd_ptr != this->_connections_fd.end())
	{
		if (!isThereSomethingToRead(*fd_ptr))
		{
			fd_ptr++;
			continue ;
		}
		int recvret = recv(*fd_ptr, buf, BUFSIZE, 0);
		if (recvret == 0)
		{
			delWatchedFd(*fd_ptr);
			close(*fd_ptr);
			this->_connections_fd.erase(fd_ptr);
			this->_request_handlers.erase(*fd_ptr);
			fd_ptr = this->_connections_fd.begin();
			continue ;
		}
		transferToBuffer(*fd_ptr, buf);
		std::cout << "in watch input : " << std::endl;
		std::cout << this->_request_handlers[*fd_ptr]->getBuffer() << std::endl;
		std::cout << "fds size : " << this->_connections_fd.size() << std::endl;
		//printf("%s\n", buf);
		FD_CLR(*fd_ptr, &Server::read_fds);
		fd_ptr++;
	}
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
