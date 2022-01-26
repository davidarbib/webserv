#include "main.hpp"

int 
processArgs(int ac, char **av, ServersType &servers, Config &conf)
{
	std::string config_path;

	std::cout << "ac : " << ac << std::endl;
	if (ac < 2)
		config_path = "default.conf";
	else
		config_path = std::string(av[1]);
	processConfigFile(servers, config_path, conf);
	return 0;
}

int
handleRequestBuffers(ServersType &servers, TicketsType &tickets,
							ReqHandlersType &request_handlers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
	{
		int ret;
		if (server->getRefConnections().size() == 0)
			return 0;
		std::map<fd_t, Connection*>::iterator it = server->getRefConnections().begin();
		for (; it != server->getRefConnections().end(); it++)
		{
			ret = parseRequest(it->second, *server, tickets, request_handlers);
		}
	}
	return 0;
}

void
handleConnectionRequest(ServersType &servers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
	{
		if (server->isThereConnectionRequest())
			server->createConnection();
	}
}

void
networkInputToBuffers(ServersType &servers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
		server->watchInput();
}

void
listenNetwork(ServersType &servers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
		server->listenSocket();
}

int
main(int ac, char **av)
{
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;
	Config						config;

	Server::max_fd = 0;
	Server::initFdset();

	processArgs(ac, av, servers, config);
	
	listenNetwork(servers);

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	while (1)
	{
		Server::setFdset();
		select(Server::max_fd + 1, &Server::read_fds, &Server::write_fds, NULL, &tv);
		handleConnectionRequest(servers);
		networkInputToBuffers(servers);
		handleRequestBuffers(servers, tickets, request_handlers);
		//writes
	}
	return 0;
}
