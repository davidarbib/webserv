#include "ConfigServer.hpp"

ConfigServer::ConfigServer(void) {
	this->_max_body = 0;
}

ConfigServer::~ConfigServer(void) {
}

ConfigServer::ConfigServer (ConfigServer const &src) {
	*this = src;
}

ConfigServer
&ConfigServer::operator=(ConfigServer const &src)
{
	this->_name = src._name;
	this->_host = src._host;
	this->_port = src._port;
	this->_error_pages = src._error_pages;
	this->_max_body = src._max_body;
	this->_locations = src._locations;
	return (*this);
}

void
ConfigServer::setAll(std::string const &confFile)
{
	int pos_start;

	if ((pos_start = confFile.find("name")) != -1)
		this->setName(parse(confFile, pos_start));
	else
		throw std::runtime_error("Missing server_name rule, missing name.");
	if ((pos_start = confFile.find("listen")) != -1)
		this->setHost(parse(confFile, pos_start));
	else
		throw std::runtime_error("Missing listen rule, missing host/port.");
	if ((pos_start = confFile.find("listen")) != -1)
		this->setPort(parse(confFile, pos_start));
	else
		throw std::runtime_error("Missing listen rule, missing host/port.");
	if ((pos_start = confFile.find("error_pages")) != -1)
		this->setErrorPages(parse(confFile, pos_start));
	else
		throw std::runtime_error("Missing error_pages rule, missing error codes/path.");
	if ((pos_start = confFile.find("client_max_body_size")) != -1)
		this->setMaxBody(parse(confFile, pos_start));
	else
		throw std::runtime_error("Missing max_body rule, missing body size.");
	if ((pos_start = confFile.find("location")) != -1)
		this->setLocations(confFile);
	else
		throw std::runtime_error("Missing location rule, missing location.");
}

//SETTERS

void
ConfigServer::setName(std::string const &name)
{
	this->_name = name;
}

void
ConfigServer::setHost(std::string const &listen)
{
	int i = 0;

	if (listen == "")
		throw std::runtime_error("Wrong listen rule format, missing host/port.");
	while (listen[i] && listen[i] != ':')
		++i;
	if (listen[i] == ':')
		this->_host = listen.substr(0, i);
	else
		throw std::runtime_error("Wrong listen block format, missing host/port.");
	struct sockaddr_in sa;
	if (inet_pton(AF_INET, this->_host.c_str(), &(sa.sin_addr)) != 1)
		throw std::runtime_error("Wrong listen block format, invalid host.");
}

void
ConfigServer::setPort(std::string const &listen)
{
	int i = 0;
	long int port;

	if (listen == "")
		throw std::runtime_error("Wrong listen rule format, missing host/port.");
	while (listen[i] && listen[i] != ':')
		++i;
	if (listen[i] != ':')
		throw std::runtime_error("Wrong listen block format, missing host/port.");
	int j = i + 1;
	while (listen[j] && listen[j] != ' ' && listen[j] != '\n')
		++j;
	this->_port = listen.substr(i + 1, j - i - 1);
	i = 0;
	while (this->_port[i])
	{
		if (this->_port[i] < '0' || this->_port[i] > '9')
			throw std::runtime_error("Wrong listen rule format, invalid port.");
		++i;
	}
	port = std::strtol(this->_port.c_str(), NULL, 10);
	if (port == 0L || port <= 0 || port > 65535)
		throw std::runtime_error("Wrong listen rule format, invalid port.");
}

void
ConfigServer::setErrorPages(std::string const &error_pages)
{
	char *str;

	if (error_pages == "")
		throw std::runtime_error("Wrong error_pages rule format, missing error codes/path.");
	str = strtok((char *)&error_pages[0], " ");
	while (str && (str[0] <= '9' && str[0] >= '0'))
	{
		this->_error_pages.errorCodes.push_back(str);
		str = strtok(NULL, " ");
	}
	if (this->_error_pages.errorCodes.size() == 0)
		throw std::runtime_error("Wrong error_pages block format, missing error codes.");
	if (str)
		this->_error_pages.path = str;
	else
		throw std::runtime_error("Wrong error_pages block format, missing path.");
	if (access(this->getErrorPages().path.c_str(), X_OK) != 0)
		throw std::runtime_error("Wrong error_pages block format, error_pages path is not a valid path for Error Pages.");

}

void
ConfigServer::setMaxBody(std::string const &max_body)
{
	if (max_body == "")
		throw std::runtime_error("Wrong max_body rule format, missing body size.");
	std::istringstream(max_body) >> this->_max_body;
}

void
ConfigServer::setLocations(std::string const &confFile)
{
	ServerLocations locations;
	std::vector<std::string> blocks;

	size_t i = 0;
	blocks = getBlocks(confFile, "location");
	if (blocks.size() == 0)
		throw std::runtime_error("Wrong location block format, missing data.");
	while (i < blocks.size())
	{
		locations = ServerLocations();
		locations.setAll(blocks[i]);
		this->_locations.push_back(locations);
		i++;
	}
}

//GETTERS

std::string
ConfigServer::getName() const {
	return (this->_name);
}

std::string
ConfigServer::getHost() const {
	return (this->_host);
}

std::string
ConfigServer::getPort() const {
	return (this->_port);
}

t_error_page
ConfigServer::getErrorPages() const {
	return (this->_error_pages);
}

int
ConfigServer::getMaxBody() const {
	return (this->_max_body);
}

std::vector<ServerLocations> const&
ConfigServer::getLocations() const {
	return (this->_locations);
}
