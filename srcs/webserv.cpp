#include "webserv.hpp"

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
	(void)ac;
	(void)av;
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;
	Response::errors_code = Response::fillResponseCodes();

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
		handleRequestBuffers(*servers[0], tickets, request_handlers);
		processRequest(tickets);
		//writes
	}
	return 0;
}
