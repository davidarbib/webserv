#include "main.hpp"

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
		if (current.getServer().getCandidateConfs()[i].getName() == current.getRequest().get_header_value("Host"))
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

	if (index != -1)
		tested_uri = original_uri;
	else
		tested_uri = resolved_uri;
	size_t extension_pos = resolved_uri.find(php_extension);
	if (extension_pos == std::string::npos)
		return false;
	if (extension_pos != resolved_uri.size() - len_extension)
		return false;
	if (access(location.getCgiPath().c_str(), X_OK) != 0)
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
parseCgiResponse(Response &response, std::string cgi_response)
{
	int index = 0;
	while (!isEndLine(cgi_response, index))
	{
		index = getHeader(index, cgi_response, response);
		if (isEndLine(cgi_response, index))
		{
			std::string body(cgi_response.substr(index, cgi_response.size()));
			std::stringstream ss;
			ss << body.size();
			response.setBody(body);
			response.setHeader("Content-Length", ss.str());
		}
	}
	std::string status_code = response.get_header_value("Status");
	if (!status_code.empty())
		return std::atoi(status_code.c_str());
	return OK;
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
		std::cout << "location choosen : " << location.getpath() << std::endl;
		int index_page_idx = -1;
		if (uri == location.getpath())
			index_page_idx = matchIndex(location, resolved_uri);
		if (executor.isValidRequest(current.getRequest(), config, location) == true)
		{
			if (isCgiRequested(uri, resolved_uri, location, index_page_idx))
			{
				executor.setStatusCode(parseCgiResponse(response,
														executor.execCgi(current.getRequest(), uri, resolved_uri,
																			query, config, location, index_page_idx)));
			}
			else if (location.getRedir().from == current.getRequest().getStartLine().request_URI)
				body_path = executor.getRedirected(location, response);
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
				body_path = executor.postMethod(current.getRequest().getStartLine().request_URI, config, location);
				response.searchForBody(executor.getStatusCode(), body_path, response.getFileExtension(body_path));
			}
			else
			{
				executor.setStatusCode(NOT_ALLOWED);
				body_path = executor.buildBodyPath(config, location.getRoot());
			}
		}
		else
			body_path = executor.buildBodyPath(config, location.getRoot());
		response.buildPreResponse(executor.getStatusCode());
		request_handlers.erase(tickets.front().getRhIt());

		//response.setHeader("Content-Length", "0"); //TODO multipart tests
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
	Response::handled_extensions = Response::fillHandledExtensions();
	ExecuteRequest::fillMethodNotImplemented();

	Server::max_fd = 0;
	Server::initFdset();
	
	processArgs(ac, av, servers, config);

	/* ------------ TODO for tests without configuration file ----------------*/
	//(void)ac;
	//(void)av;
	//ConfigServer conf;
	//conf.setName("127.0.0.1:8003");
	//conf.setHost("127.0.0.1:8003");
	//conf.setPort("8003");
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
		processRequest(tickets, request_handlers);
		sendToNetwork(servers);
	}
	return 0;
}
