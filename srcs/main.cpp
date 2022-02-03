#include "main.hpp"

int 
processArgs(int ac, char **av, ServersType &servers, Config &conf)
{
	std::string config_path;

	std::cout << "ac : " << ac << std::endl;
	if (ac < 2)
		config_path = "webserv.conf";
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
networkInputToBuffers(ServersType &servers, ReqHandlersType &request_handlers)
{
	ServersType::iterator server = servers.begin();
	for (; server != servers.end(); server++)
		server->watchInput(request_handlers);
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

ConfigServer const &
getConfig(Ticket current)
{
	for (size_t i = 0; i < current.getServer().getCandidateConfs().size(); i++)
	{
		if (current.getServer().getCandidateConfs()[i].getName() == current.getRequest().get_header_value("Host"))
			return current.getServer().getCandidateConfs()[i];
	}
	return ConfigServer();
}

Response
processRequest(TicketsType &tickets)
{
	ExecuteRequest executor;
	Response response;
	std::string body_path;
	while (!tickets.empty() && tickets.front().getRequest().isRequestFinalized() == true)
	{
		Ticket current(tickets.front());
		ConfigServer const& config = getConfig(current);
		if (executor.isValidRequest(current.getRequest(), config) == true)
		{
			if (current.getRequest().getStartLine().method_token == "DELETE")
				body_path = executor.deleteMethod(current.getRequest().getStartLine().request_URI, config);
			else if (current.getRequest().getStartLine().method_token == "GET")
				body_path = executor.getMethod(current.getRequest().getStartLine().request_URI, config);
		}
		response.buildPreResponse(executor.getStatusCode(), body_path);
		//std::cout << response.serialize_response() << std::endl;
		tickets.front().getConnection() << response.serialize_response();
		tickets.pop();
	}
	return response;
}

int
main(int ac, char **av)
{
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;
	Config						config;

	Response::errors_code = Response::fillResponseCodes();
	ExecuteRequest::fillMethodNotImplemented();

	Server::max_fd = 0;
	Server::initFdset();

	
	processArgs(ac, av, servers, config);

	/* ------------ TODO for tests without configuration file ----------------*/
	//(void)ac;
	//(void)av;
	//ConfigServer conf;
	//conf.setName("127.0.0.1:8003");
	//// conf.setHost("127.0.0.1:8003");
	//// conf.setPort("8003");
	//conf.setMaxBody("200");
	//std::vector<ConfigServer> configs;
	//configs.push_back(conf);
	//servers.push_back(Server("127.0.0.1", "8003", configs));

	/* -----------------------------------------------------------------------*/
	
	listenNetwork(servers);

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	while (1)
	{
		Server::setFdset();
		select(Server::max_fd + 1, &Server::read_fds, &Server::write_fds, NULL, &tv);
		handleConnectionRequest(servers);
		networkInputToBuffers(servers, request_handlers);
		handleRequestBuffers(servers, tickets, request_handlers);
		processRequest(tickets);
		sendToNetwork(servers);
	}
	return 0;
}
