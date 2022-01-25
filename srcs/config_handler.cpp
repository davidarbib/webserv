#include "config_handler.hpp"

typedef std::multimap<HostPort, ServerConfig&> conf_mm;
typedef std::vector<ServerConfig&> config_v;
typedef std::vector<HostPort> hostport_v;

int
createCandidateConfigsList(conf_mm &hostport_configs,
							hostport_v::iterator hostport, config_v &candidates)
{
	std::pair<conf_mm::iterator, conf_mm::iterator> range;

	range = hostport_configs.equal_range(*hostport);
	for (
	
}

int
createServers(ServersType &servers, conf_mm &hostport_configs,
				std::vector<HostPort> &hostports)
{
	for (std::vector<HostPort>::iterator hostport = hostports.begin();
			hostport != hostports.end(); hostport++)
	{
		config_v	candidate_configs;
		createCandidateConfigsList(hostport_configs, candidates);
		
	}
}

int
classConfigs(conf_mm &hostport_configs, std::vector<HostPort> &hostports)
{
	std::vector<ConfigServer> &configs = conf.getServers();
	for (it = std::vector<ConfigServer>::iterator = configs.begin();
			it != configs.end(); it++)
	{
		std::pair<HostPort, ServerConfig&> pair;
		HostPort hostport = HostPort(it->getHost(), it->getPort());
		pair = std::make_pair(hostport, *it);
		hostport_configs.insert(pair);
		hostports.push_back(hostport);
	}
}

int 
processConfigFile(std::string &config_path, Config &conf)
{	
	conf_mm					hostport_configs;
	std::vector<HostPort>	hostports;
	std::vector<HostPort>	unique_hostports;

	conf.setServers(config_path);
	classConfigs(hostport_configs, hostports);
	std::vector<HostPort>::iterator hp_end_it;
	hp_end_it = std::unique_copy(hostports.begin(), hostports.end(),
									unique_hostports.begin())
	createServers(servers, hostport_configs, hostports);
	

	return 0;
}

