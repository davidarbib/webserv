#include "Server.hpp"

int parseRequest(Server &server)
{
	std::map<fd_t, std::string>::iterator it;
	for (it = server.getRequestBuffers().begin();
			it != server.getRequestBuffers().end();
			it++)
	{
	}
}

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
		parseRequest(servers[0]);
		//Maxime's stuff
		//writes
	}
	return 0;
}

/*
typedef struct s_bool
{
	int b : 1;
}				t_bool;
*/
