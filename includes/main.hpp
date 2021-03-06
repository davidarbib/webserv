#ifndef MAIN_HPP
# define MAIN_HPP
	
# include "Server.hpp"
# include "request_parser.hpp"
# include "parsing_tools.hpp"
# include "config_handler.hpp"
# include "RequestHandler.hpp"
# include "Ticket.hpp"
# include <deque>
# include <queue>
# include <unistd.h>
#include "Response.hpp"
#include "ExecuteRequest.hpp"
#include "ServerLocations.hpp"
#include <csignal>
#include <algorithm>

typedef std::queue<Ticket>						TicketsType;
typedef std::map<fd_t, RequestHandler>			ReqHandlersType;

#endif
