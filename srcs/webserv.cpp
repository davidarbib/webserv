#include "Server.hpp"

#define _LIBCPP_DEBUG 1

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	struct timeval		tv;
	std::vector<Server*> servers;

	Server::max_fd = 0;
	Server::initFdset();
	servers.push_back(new Server("origin", PORT, "/tmp/access", "/tmp/error"));
	
	servers[0]->listenSocket();

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	while (1)
	{
		Server::setFdset();
		select(Server::max_fd + 1, &Server::read_fds, &Server::write_fds, NULL, &tv);
		if (servers[0]->isThereConnectionRequest())
			servers[0]->createConnection();
		servers[0]->watchInput();
	}
	return 0;
}
