#ifndef MAIN_HPP
# define MAIN_HPP
	
# include "Server.hpp"
# include "request_parser.hpp"
# include "config_handler.hpp"
# include "RequestHandler.hpp"
# include "Ticket.hpp"
# include <deque>
# include <queue>

typedef std::queue<Ticket>						TicketsType;
typedef std::vector<Server>						ServersType;
typedef std::map<fd_t, RequestHandler>			ReqHandlersType;

#endif
