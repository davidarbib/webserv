#include "Server.hpp"
#include "request_parser.hpp"
#include "RequestHandler.hpp"
#include <deque>

int handleRequestBuffers(Server &server)
{
	//int ret;

	if (server.getRefConnections().size() == 0)
		return 0;
	//std::map<fd_t, Connection*>::iterator it = server.getRefConnections().begin();
	//for (; it != server.getRefConnections().end(); it++)
	//	ret = parseRequest(it->second->getInBuffer(), server);
	return 0;
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	struct timeval		tv;
	std::vector<Server*> servers;
	std::deque<RequestHandler> request_handlers;

	Server::max_fd = 0;
	Server::initFdset();
	servers.push_back(new Server("origin", IP, PORT, "/tmp/access", "/tmp/error"));
	
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
		handleRequestBuffers(*servers[0]);
		//writes
	}
	return 0;
}
