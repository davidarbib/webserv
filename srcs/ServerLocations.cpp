#include "ServerLocations.hpp"

ServerLocations::ServerLocations(void) {
	this->_auto_index = 0;
}

ServerLocations::~ServerLocations(void) {
}

ServerLocations::ServerLocations (ServerLocations const &src) {
	*this = src;
}

void
ServerLocations::setAll(std::string const &confFile)
{
	int pos_start;

	if ((pos_start = confFile.find("location")) != -1)
		this->setPath(parse(confFile, pos_start));
	if ((pos_start = confFile.find("methods_allowed")) != -1)
		this->setMethods(parse(confFile, pos_start));
	if ((pos_start = confFile.find("auto_index")) != -1)
		this->setAuto_index(parse(confFile, pos_start));
	if ((pos_start = confFile.find("root")) != -1)
		this->setRoot(parse(confFile, pos_start));
	if ((pos_start = confFile.find("index")) != -1)
		this->setIndex(parse(confFile, pos_start));
	if ((pos_start = confFile.find("redir")) != -1)
		this->setRedir(parse(confFile, pos_start));
	if ((pos_start = confFile.find("cgi_path")) != -1)
		this->setCgi_path(parse(confFile, pos_start));
}

//SETTERS

void
ServerLocations::setPath(std::string const &path)
{
	char *str;

	str = strtok((char *)&path[0], " ");
	if (str && str[0] != '{')
		this->_path = str;
	else
		throw("Wrong location block format, missing root directive.");
}

static void print_vector(std::vector<std::string> vector) {
	int i;

	i = 0;
	while (i < vector.size())
	{
		std::cout << vector[i] << " ";
		i++;
	}
	std::cout << std::endl;
}

void
ServerLocations::setMethods(std::string const &methods)
{
	char *str;
	std::string lst[3] = {"GET", "POST", "DELETE"};

	if (methods == "")
		throw("Wrong methods rule format, missing methods.");
	str = strtok((char *)&methods[0], " ");
	while (str)
	{
		this->_methods.push_back(str);
		str = strtok(NULL, " ");
	}
	size_t i = 0;
	while (i < this->_methods.size())
	{
		if (std::find(lst, lst + lst->size(), this->_methods[i]) == lst + lst->size())
			throw("Wrong methods rule format, unknown method name.");
		i++;
	}
}

void
ServerLocations::setAuto_index(std::string const &auto_index)
{
	if (auto_index == "")
		throw("Wrong auto_index rule format, missing auto index param.");
	else if (auto_index == "on")
		this->_auto_index = 1;
	else if (auto_index == "off")
		this->_auto_index = 0;
	else
		throw("Wrong auto_index rule format, auto_index param not valid.");
}

void
ServerLocations::setIndex(std::string const &index)
{
	char *str;

	if (index == "")
		throw("Wrong index rule format, missing index.");
	str = strtok((char *)&index[0], " ");
	while (str)
	{
		this->_index.push_back(str);
		str = strtok(NULL, " ");
	}
}

void
ServerLocations::setRoot(std::string const &root)
{
	if (root == "")
		throw("Wrong root rule format, missing root.");
	this->_root = root;
}

void
ServerLocations::setCgi_path(std::string const &cgi_path)
{
	if (cgi_path == "")
		throw("Wrong cgi_path rule format, missing path.");
	this->_cgi_path = cgi_path;
}

void
ServerLocations::setRedir(std::string const &redir)
{
	char *str;

	if (redir == "")
		throw("Wrong redir rule format, missing redir.");
	str = strtok((char *)&redir[0], " ");
	this->_redir.from = str;
	str = strtok(NULL, " ");
	this->_redir.to = str;
}

//GETTERS

std::string
ServerLocations::getpath() const {
	return (this->_path);
}

std::vector<std::string>
ServerLocations::getMethods() const {
	return (this->_methods);
}

std::string
ServerLocations::getRoot() const {
	return (this->_root);
}

std::vector<std::string>
ServerLocations::getIndex() const {
	return (this->_index);
}

std::string
ServerLocations::getCgi_path() const {
	return (this->_cgi_path);
}

int
ServerLocations::getAuto_index() const {
	return (this->_auto_index);
}

s_redir
ServerLocations::getRedir() const {
	return (this->_redir);
}

ServerLocations
&ServerLocations::operator=(ServerLocations const &src)
{
	this->_path = src._path;
	this->_methods = src._methods;
	this->_root = src._root;
	this->_index = src._index;
	this->_cgi_path = src._cgi_path;
	this->_auto_index = src._auto_index;
	this->_redir = src._redir;
	return (*this);
}
