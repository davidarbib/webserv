#include "Config.hpp"
#include <unistd.h>

void print_vector(std::vector<std::string> vector) {
	for (std::vector<std::string>::iterator i = vector.begin(); i != vector.end(); ++i)
	    std::cout << *i << ' ';
	std::cout << std::endl;
}

int set_server_config_test()
{
	Config conf;
    ConfigServer server;
	ServerLocations location;
	std::string confFile;

	if ((confFile = read_config("../tests/webserv.conf")) == "")
		return (-1);

	try {
		conf.setServers(confFile);
	}
	catch(char const *error) {
		std::cerr << "Error. Wrong configuration, please provide a valid \"webserv.conf\" file:" << std::endl;
		std::cerr << error << std::endl;
		return (-1);
	}

	server = conf.getServers().front();

	std::cout << "--- Parsing server ---" << std::endl; 
	
	std::cout << "name: " << server.getName() << std::endl;
	std::cout << "host: " << server.getHost() << std::endl;
	std::cout << "port: " << server.getPort() << std::endl;
	std::cout << "error_pages (codes): ";
	print_vector(server.getErrorPages().errorCodes);
	std::cout << "error_pages (path): " << server.getErrorPages().path << std::endl;
	std::cout << "client_max_body_size: " << server.getMax_body() << std::endl << std::endl;

	location = server.getLocations().front();

	std::cout << "--- Parsing location ---" << std::endl;
	
	std::cout << "path: " << location.getpath() << std::endl;
	std::cout << "methods: ";
	print_vector(location.getMethods());
	std::cout << "index: ";
	print_vector(location.getIndex());
	std::cout << "auto_index: " << location.getAuto_index() << std::endl;
	std::cout << "redir (from): " << location.getRedir().from << std::endl;
	std::cout << "redir (to): " << location.getRedir().to << std::endl;
	std::cout << "cgi_path: " << location.getCgi_path() << std::endl;

	return 0;
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	
	set_server_config_test();
	return (0);
}