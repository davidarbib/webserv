#ifndef CONFIG_HANDLER_HPP
# define CONFIG_HANDLER_HPP

# include "Config.hpp"
# include "Server.hpp"

int 
processConfigFile(std::string &config_path, Config &conf);

int
createServers(ServersType &servers, Config &conf);

#endif
