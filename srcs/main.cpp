#include "main.hpp"
#define D_SIZE 45000

int
processArgs(int ac, char **av, ServersType &servers, Config &conf)
{
	std::string config_path;

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
		if (current.getServer().getCandidateConfs()[i].getName() == current.getRequest().getHeaderValue("Host"))
			return current.getServer().getCandidateConfs()[i];
	}
	return current.getServer().getCandidateConfs()[0];
}

size_t
matchLocation(std::string const& location, std::string const& uri)
{
	size_t matching_len = 0;

	while (matching_len < location.size() && matching_len < uri.size() && location[matching_len] == uri[matching_len])
		matching_len++;
	return matching_len;
}

ServerLocations const&
getLocation(ConfigServer const& config, std::string const& uri)
{
	size_t max_match = 0;
	size_t current_match = 0;
	size_t matched_index = 0;
	for (size_t i = 0; i < config.getLocations().size(); i++)
	{
		current_match = matchLocation(config.getLocations()[i].getpath(), uri);
		if (current_match > max_match)
		{
			max_match = current_match;
			matched_index = i;
		}
	}
	return config.getLocations()[matched_index];
}

bool
isCgiRequested(std::string const &original_uri, std::string const &resolved_uri, ServerLocations const &location, int index)
{
	std::string php_extension = ".php";
	size_t len_extension = php_extension.size();
	std::string tested_uri;

	if (index == -1)
		tested_uri = original_uri;
	else
		tested_uri = resolved_uri;
	size_t extension_pos = tested_uri.find(php_extension);
	if (extension_pos == std::string::npos)
		return false;
	if (extension_pos != tested_uri.size() - len_extension)
		return false;
	if (location.getCgiPath().empty())
		return false;
	return true;
}

int
matchIndex(ServerLocations const &location, std::string &resolved_uri)
{
    for (size_t i = 0; i < location.getIndex().size(); i++)
    {
        resolved_uri = location.getRoot() + "/" + location.getIndex()[i];
		if (access(resolved_uri.c_str(), F_OK) == 0)
			return static_cast<int>(i);
	}
	return -1;
}

# define QUERYCHAR '?'

void
cutQuery(Request &request, std::string &query)
{
	size_t query_pos = request.getStartLine().request_URI.find(QUERYCHAR);

	query = request.getStartLine().request_URI.substr(query_pos + 1);
	request.setRequestURI(request.getStartLine().request_URI.substr(0, query_pos));
}

int
parseCgiResponse(Response &response, AHttpMessage::body_type cgi_response)
{
	int index = 0;
	while (!isEndLine(cgi_response, index))
	{
		std::string tmp;
		tmp.reserve(cgi_response.size());
		tmp.assign(cgi_response.begin(), cgi_response.end());
		index = getHeader(index, tmp, response);
		if (isEndLine(cgi_response, index))
		{
			AHttpMessage::body_type body;
			body.insert(body.begin(), cgi_response.begin() + index, cgi_response.end());
			std::stringstream ss;
			ss << body.size();
			response.setBody(body);
			response.setHeader("Content-Length", ss.str());
		}
	}
	std::string status_code = response.getHeaderValue("Status");
	if (!status_code.empty())
		return std::atoi(status_code.c_str());
	return OK;
}

#define CONTINUE_VALUE "100-continue"

bool
is100Continue(Request const &request)
{
	if (request.getHeaderValue("Expect") == CONTINUE_VALUE)
		return true;
	return false;
}

Response
processRequest(TicketsType &tickets, ReqHandlersType &request_handlers)
{
	ExecuteRequest executor;
	Response response;
	std::string body_path;
	
	while (!tickets.empty() && tickets.front().getRequest().isRequestFinalized() == true)
	{
		Ticket current(tickets.front());
		ConfigServer const& config = getConfig(current);
		std::string query;
		cutQuery(current.getRequest(), query);
		std::string uri = current.getRequest().getStartLine().request_URI;
		std::string resolved_uri;
		ServerLocations const& location = getLocation(config, uri);
		int index_page_idx = -1;
		if (uri == location.getpath())
			index_page_idx = matchIndex(location, resolved_uri);
		if (executor.isValidRequest(current.getRequest(), config, location) == true)
		{
			std::string query;
			cutQuery(current.getRequest(), query);
			if (is100Continue(current.getRequest()))
				body_path = executor.continueGeneration(current);
			if (isCgiRequested(uri, resolved_uri, location, index_page_idx))
			{

				try
				{
					AHttpMessage::body_type cgi_exec = executor.execCgi(current.getRequest(), uri, resolved_uri, query, location, index_page_idx);
					int cgi_response = parseCgiResponse(response, cgi_exec);
					executor.setStatusCode(cgi_response);
				}
				catch(std::exception& e)
				{
					executor.setStatusCode(INTERNAL_SERVER_ERROR);
					body_path = executor.buildBodyPath(config);
					response.searchForBody(executor.getStatusCode(), body_path, response.getFileExtension(body_path));
				}
			}
			else if (location.getRedir().from == current.getRequest().getStartLine().request_URI)
			{
				body_path = executor.getRedirected(location, response);
			}
			else if (current.getRequest().getStartLine().method_token == "DELETE")
			{
				body_path = executor.deleteMethod(current.getRequest().getStartLine().request_URI, config, location, resolved_uri);
				response.searchForBody(executor.getStatusCode(), body_path, response.getFileExtension(body_path));
			}
			else if (current.getRequest().getStartLine().method_token == "GET")
			{
				body_path = executor.getMethod(current.getRequest().getStartLine().request_URI, config, location, resolved_uri);
				response.searchForBody(executor.getStatusCode(), body_path, response.getFileExtension(body_path));
			}
			else if (current.getRequest().getStartLine().method_token == "POST")
			{
				body_path = executor.postMethod(current.getRequest().getStartLine().request_URI, config, location, current);
				response.searchForBody(executor.getStatusCode(), body_path, response.getFileExtension(body_path));
			}
			else
			{
				executor.setStatusCode(NOT_ALLOWED);
				body_path = executor.buildBodyPath(config);
			}
		}
		else
			body_path = executor.buildBodyPath(config);
		std::cout << "RESPONSE CODE :" << executor.getStatusCode() << std::endl;
		response.buildPreResponse(executor.getStatusCode());
		request_handlers.erase(tickets.front().getRhIt());
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
	Response::handled_extensions = Response::fillHandledExtensions();
	ExecuteRequest::fillMethodNotImplemented();

	Server::max_fd = 0;
	Server::initFdset();

	try {
		processArgs(ac, av, servers, config);
	}
	catch(const std::runtime_error& e) {
		std::cerr << "Error. Wrong configuration, please provide a valid \"webserv.conf\" file." << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}

	try
	{
		listenNetwork(servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what();
		exit(1);
	}
	
	tv.tv_sec = DELAY;
	tv.tv_usec = 0;

	while (1)
	{
		Server::setFdset();
		select(Server::max_fd + 1, &Server::read_fds, &Server::write_fds, NULL, &tv);
		handleConnectionRequest(servers);
		networkInputToBuffers(servers, request_handlers);
		handleRequestBuffers(servers, tickets, request_handlers);
		processRequest(tickets, request_handlers);
		sendToNetwork(servers);
	}
	return 0;
}
