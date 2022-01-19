#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <string.h>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include <cstdlib>

# include "ConfigServer.hpp"

class ConfigServer;

class Config {

private:
	std::vector<ConfigServer>	_servers;

public:
	Config(void);
	Config(Config const &src);
	~Config(void);

	Config	&operator=(Config const &src);

	//SETTERS
	void	setServers(std::string confFile);

	//GETTERS
	std::vector<ConfigServer> &
	getServers() const;
};
#endif
