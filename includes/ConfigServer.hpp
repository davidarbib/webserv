#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include "ServerLocations.hpp"

# include <vector>
# include <string>
# include <string.h>
# include <sstream>

# include "ConfigUtils.hpp"

typedef struct  s_error_page {

	std::vector<std::string>	errorCodes;
	std::string			path;

}	t_error_page;

class ConfigServer {

public:
	ConfigServer(void);
	ConfigServer(ConfigServer const &src);
	~ConfigServer(void);

	ConfigServer
	&operator=(ConfigServer const &src);

	//SETTERS
	void
	setAll(std::string const &confFile);

	void
	setName(std::string const &name);

	void
	setHost(std::string const &listen);

	void
	setPort(std::string const &listen);

	void
	setErrorPages(std::string const &error_pages);

	void
	setMax_body(std::string const &max_body);

	void
	setLocations(std::string const &locations);

	//GETTERS
	std::string
	getName() const;

	std::string
	getHost() const;

	std::string
	getPort() const;

	t_error_page
	getErrorPages() const;

	int
	getMax_body() const;

	std::vector<ServerLocations>
	getLocations() const;

private:
	std::string						_name;
	std::string						_host;
	std::string						_port;
	t_error_page					_error_pages;
	int								_max_body;

	std::vector<ServerLocations> 	_locations;
};
#endif
