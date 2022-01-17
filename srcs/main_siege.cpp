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

bool
quickparser(Buffer &buffer) // TODO debug purpose only, problem for big request
{
	for (unsigned long i = 0; i < buffer.getBuffer().size() - CRLFCRLF; i++)
	{
		if (isEndSection(buffer.getBuffer(), i))
			return true;
		else
			buffer.getBuffer()[i] = 0;
	}
	return false;
}

#define CRLF_S "\r\n"

void
quickresponse(Server &server) //TODO copy, modify and integrate and delete after debug
{
	std::string response;
	std::stringstream stream;
	
	stream << "HTTP/1.1 200 OK" << CRLF_S;
	stream << "Content-Length: 2" << CRLF_S;
	stream << "Content-Type: text/plain" << CRLF_S;
	stream << CRLF_S << CRLF_S;
	stream << "cc" << CRLF_S;

	stream >> response;

	//Date: Mon, 27 Jul 2009 12:28:53 GMT
	//Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
	//Vary: Accept-Encoding

	if (server.getRefConnections().size() == 0)
		return ;
	std::map<fd_t, Connection*>::iterator it = server.getRefConnections().begin();
	for(; it != server.getRefConnections().end(); it++)
	{	
		Connection* connection = it->second;
	//	connection->setPendingRequest(quickparser(connection->getInBuffer()));

	//	if (connection->_pending_request 
	//		&& server.isWritePossible(connection->getSocketFd()))
		if (server.isWritePossible(connection->getSocketFd()))
		{
			write(connection->getSocketFd(), response.c_str(), 1000);
			connection->setPendingRequest(false);
		}
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	struct timeval				tv;
	std::vector<Server*>		servers;
	std::deque<RequestHandler>	request_handlers;

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
		//handleRequestBuffers(*servers[0]);
		quickresponse(*servers[0]);
		//writes
	}
	return 0;
}
