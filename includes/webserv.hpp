#ifndef WEBSERV_HPP
# define WEBSERV_HPP
	
# include "Server.hpp"
# include "request_parser.hpp"
# include "RequestHandler.hpp"
# include "Ticket.hpp"
# include <deque>
# include <queue>
#include "Response.hpp"

typedef std::queue<Ticket>						TicketsType;
typedef std::vector<Server*>					ServersType;
typedef std::map<fd_t, RequestHandler>			ReqHandlersType;

#endif
