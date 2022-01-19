#include "main.hpp"

int processArgs(int ac, char **av, Config &conf)
{
	if (ac < 1)
		std::string config_path = "default.conf";
	else
		std::string config_path = std::string(av[0]);
	processConfigFile(config_path);
	return 0;
}

int processConfigFile(std::string &config_path, Config &conf)
{	
	conf.setServers(config_path);	
	return 0;
}

int createServers(ServersType &servers, Config &conf)
{
	std::vector<ConfigServer> &configs = conf.getServers();
	for (it = std::vector<ConfigServer>::iterator = configs.begin();
			it != configs.end(); it++)
		servers.push_back(new Server("origin", IP, PORT, "/tmp/access", "/tmp/error", *it));
}

int handleRequestBuffers(Server &server, TicketsType &tickets,
							ReqHandlersType &request_handlers)
{
	int ret;
	if (server.getRefConnections().size() == 0)
		return 0;
	std::map<fd_t, Connection*>::iterator it = server.getRefConnections().begin();
	for (; it != server.getRefConnections().end(); it++)
		ret = parseRequest(it->second, server, tickets, request_handlers);
	return 0;
}

int main(int ac, char **av)
{
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;
	Config						conf;

	Server::max_fd = 0;
	Server::initFdset();

	processArgs(ac, av, conf);
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
		handleRequestBuffers(*servers[0], tickets, request_handlers);
		//writes
	}
	return 0;
}
