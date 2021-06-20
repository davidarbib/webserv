#include "Server.hpp"

int main(int ac, char **av)
{
	struct timeval		tv;
	fd_t				max_fd;
	std::vector<Server> servers;

	Server::max_fd = 0;
	Server::initFdset();
	servers.push_back(new Server("origin", PORT, "/tmp/access", "/tmp/error"));
	
	servers[0].listen();

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	while (1)
	{
		Server::setFdset();
		select(Server::maxfd + 1, &Server::read_fds, &Server::write_fds, NULL, &tv);
		if (servers[0].theresConnectionRequest())
			servers[0].createConnection();
		servers[0].watchInput();
	}
	return 0;
}
