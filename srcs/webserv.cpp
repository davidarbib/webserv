#include "Server.hpp"
#include "request_parser.hpp"

int handleRequestBuffers(Server *server)
{
	std::map<fd_t, RequestHandler*> request_handlers = server->getRefRequestHandlers();
	int ret;

	if (request_handlers.size() == 0)
		return 0;
	std::map<fd_t, RequestHandler*>::iterator it;
	for (it = request_handlers.begin();
			it != request_handlers.end();
			it++)
	ret = parseRequest(it, server);
	return ret;
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
		handleRequestBuffers(servers[0]);
		//writes
	}
	return 0;
}
