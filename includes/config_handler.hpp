#ifndef CONFIG_HANDLER_HPP
# define CONFIG_HANDLER_HPP

# include "Config.hpp"
# include "Server.hpp"
# include "HostPort.hpp"
# include <algorithm>

typedef std::vector<Server>						ServersType;

int 
processConfigFile(ServersType &servers, std::string &config_path, Config &conf);

#endif
