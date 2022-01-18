#include "Server.hpp"
#include "request_parser.hpp"
#include "RequestHandler.hpp"
#include <deque>
#include <stdio.h>

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

#define CRLF_S "\r\n"

void
quickresponse(TicketsType &tickets) //TODO copy, modify and integrate and delete after debug
{
	char s[120];
	
	memset(s, 0, 120);
	sprintf(s, "HTTP/1.1 200 OK%sContent-Length: 2%sContent-Type: text/plain%s%scc%s%s", 
			CRLF_S, CRLF_S, CRLF_S, CRLF_S, CRLF_S, CRLF_S);
	while (!tickets.empty())
	{
		Ticket &current_ticket = tickets.front();
		Connection const &connection = current_ticket.getConnection();
		if (current_ticket.getServer().isWritePossible(connection.getSocketFd()))
		{
			int ret = write(connection.getSocketFd(), s, strlen(s));
			std::cout << "ret : " << ret << std::endl;
		}
		tickets.pop();
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	struct timeval				tv;
	ServersType					servers;
	ReqHandlersType				request_handlers;
	TicketsType					tickets;

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
		{
			std::cout << "connection requested" << std::endl;
			servers[0]->createConnection();
		}
		servers[0]->watchInput(request_handlers);
		handleRequestBuffers(*servers[0], tickets, request_handlers);
		quickresponse(tickets);
		//writes
	}
	return 0;
}
