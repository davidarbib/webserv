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

void
sendToNetwork(ServersType &servers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
		server->send();
}

Response processRequest(TicketsType &tickets)
{
	ExecuteRequest executor;
	Response response;
	std::string body_path;
	while (!tickets.empty() && tickets.front().getRequest().isRequestFinalized() == true)
	{
		if (executor.isValidRequest(tickets.front().getRequest()) == true)
		{
			if (tickets.front().getRequest().getStartLine().method_token == "DELETE")
				body_path = executor.deleteMethod(tickets.front().getRequest().getStartLine().request_URI);
			else if (tickets.front().getRequest().getStartLine().method_token == "GET")
				body_path = executor.getMethod(tickets.front().getRequest().getStartLine().request_URI);
			else
			{
				executor.setStatusCode(405);
				body_path = executor.buildBodyPath();
			}
		}
		std::cout << "STATUS CODE : " << executor.getStatusCode() << std::endl;
		response.buildPreResponse(executor.getStatusCode(), body_path);
		std::cout << response.serialize_response() << std::endl;
		tickets.pop();
	}
	return response;
}

int main(int ac, char **av)
{
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;
	Config						config;

	Response::errors_code = Response::fillResponseCodes();

	Server::max_fd = 0;
	Server::initFdset();

	
	/* ------------ TODO for tests without configuration file ----------------*/
	//processArgs(ac, av, servers, config);
	(void)ac;
	(void)av;
	servers.push_back(Server("127.0.0.1", "8003", std::vector<ConfigServer>()));

	/* -----------------------------------------------------------------------*/
	
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
		processRequest(tickets);
		sendToNetwork(servers);
	}
	return 0;
}
