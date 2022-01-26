#include "Config.hpp"

Config::Config(void) {
}

Config::~Config(void) {
	
}

Config::Config (Config const &src) {
	*this = src;
}

Config
&Config::operator=(Config const &src)
{
	this->_servers = src._servers;
	return (*this);
}

//SETTERS

void
Config::setServers(std::string confFile)
{
	ConfigServer servers;
	std::vector<std::string> blocks;

	size_t i = 0;
	blocks = getBlocks(confFile, "server");
	if (blocks.size() == 0)
		throw ("Wrong server block format, missing data.");
	while (i < blocks.size())
	{
		servers.setAll(blocks[i]);
		this->_servers.push_back(servers);
		i++;
	}
}

//GETTERS

std::vector<ConfigServer> const &
Config::getServers() const
{
	return (this->_servers);
}
