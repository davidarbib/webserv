#include "config_handler.hpp"

typedef std::multimap<HostPort, ConfigServer>	conf_mm;
typedef std::vector<ConfigServer>				config_v;
typedef std::vector<HostPort>					hostport_v;

int
createCandidateConfigsList(conf_mm &hostport_configs,
							hostport_v::iterator hostport, config_v &candidates)
{
	std::pair<conf_mm::iterator, conf_mm::iterator> candidates_conf_range;

	candidates_conf_range = hostport_configs.equal_range(*hostport);
	for (conf_mm::iterator it = candidates_conf_range.first;
			it != candidates_conf_range.second; it++)
		candidates.push_back(it->second);
	return 0;
}

int
createServers(ServersType &servers, conf_mm &hostport_configs,
				std::vector<HostPort> &hostports)
{
	for (std::vector<HostPort>::iterator hostport = hostports.begin();
			hostport != hostports.end(); hostport++)
	{
		config_v	candidate_configs;
		createCandidateConfigsList(hostport_configs, hostport, candidate_configs);
		servers.push_back(Server(hostport->getIp(), hostport->getPort(),
									candidate_configs));	
	}
	return 0;
}

int
classConfigs(Config &conf, conf_mm &hostport_configs,
				std::vector<HostPort> &hostports)
{
	config_v const &configs = conf.getServers();
	for (config_v::const_iterator it = configs.begin();
			it != configs.end(); it++)
	{
		HostPort hostport = HostPort(it->getHost(), it->getPort());
		hostport_configs.insert(std::make_pair(hostport, *it));
		hostports.push_back(hostport);
	}
	return 0;
}

int 
processConfigFile(ServersType &servers, std::string &config_path, Config &conf)
{	
	conf_mm					hostport_configs;
	std::vector<HostPort>	hostports;
	std::string				conf_file;

	if ((conf_file = read_config(config_path)) == "")
		return (-1);
	try {
		conf.setServers(conf_file);
	}
	catch(char const *error) {
		std::cerr << "Error. Wrong configuration, please provide a valid \"webserv.conf\" file:" << std::endl;
		std::cerr << error << std::endl;
		return (-1);
	}
	//TODO config file error handling
	classConfigs(conf, hostport_configs, hostports);
	std::vector<HostPort>::iterator hp_end_it;
	std::vector<HostPort>	unique_hostports(hostports.size());
	hp_end_it = std::unique_copy(hostports.begin(), hostports.end(),
									unique_hostports.begin());
	unique_hostports.resize(std::distance(unique_hostports.begin(), hp_end_it));
	createServers(servers, hostport_configs, unique_hostports);
	return 0;
}

