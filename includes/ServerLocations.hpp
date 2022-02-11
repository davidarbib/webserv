#ifndef SERVERLOCATIONS_HPP
# define SERVERLOCATIONS_HPP

# include <string>
# include <string.h>
# include <vector>
# include <iostream>

# include "ConfigUtils.hpp"

typedef struct  s_redir {

	std::string	from;
	std::string	to;

}	t_redir;

class ServerLocations {

public:
	ServerLocations(void);
	ServerLocations(ServerLocations const &src);
	~ServerLocations(void);

	ServerLocations
	&operator=(ServerLocations const &src);

	//SETTERS

	void
	setAll(std::string const &confFile);

	void
	setPath(std::string const &path);

	void
	setMethods(std::string const &methods);

	void
	setRoot(std::string const &root);

	void
	setIndex(std::string const &index);

	void
	setAutoIndex(std::string const &auto_index);

	void
	setCgiPath(std::string const &cgi_path);

	void
	setRedir(std::string const &redir);

	//GETTERS

	std::string
	getpath() const;

	std::vector<std::string>
	getMethods() const;

	std::string
	getRoot() const;

	std::vector<std::string>
	getIndex() const;
	
	std::string const&
	getCgiPath() const;

	int
	getAutoIndex() const;

	s_redir
	getRedir() const;

private:
	std::string						_path;
	std::vector<std::string>		_methods;
	std::string						_root;
	std::vector<std::string>		_index;
	std::string						_cgi_path;
	int								_auto_index;
	s_redir							_redir;
};

#endif
