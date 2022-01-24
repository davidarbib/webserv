#include "config_handler.hpp"

int
createServers(ServersType &servers, Config &conf)
{
	std::vector<ConfigServer> &configs = conf.getServers();
	for (it = std::vector<ConfigServer>::iterator = configs.begin();
			it != configs.end(); it++)
		servers.push_back(Server("origin", IP, PORT, "/tmp/access", "/tmp/error", *it));
}

int 
processConfigFile(std::string &config_path, Config &conf)
{	
	std::multimap<HostPort, ServerConfig &> hostport_configs;
	
	conf.setServers(config_path);
	return 0;
}

