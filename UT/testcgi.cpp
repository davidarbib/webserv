#include "CgiHandler.hpp"
#include "Request.hpp"
#include <iostream>

int main()
{
	Request request;

	request.set_method_token("POST");
	request.set_request_URI("www.example.com/test?querystring");
	request.set_http_version("1.1");
	request.set_body("Aloha");
	request.set_header("Content-Length", "5");
	request.set_header("Content-Type", "text/html");
	CgiHandler handler(request, new std::string("/usr/bin/php-cgi"),
						new std::string("/home/daav/webserv/UT/index.php"));

	std::cout << "---------------------Cgi Env---------------------" << std::endl;
	char **cgi_env = handler.getCgiEnv();
	int i = 0;
	while (cgi_env[i])
		std::cout << cgi_env[i++] << std::endl;
	int j = 0;
	while (cgi_env[j])
		delete [] cgi_env[j++];
	delete [] cgi_env;

	std::cout << "---------------------Exec CGI---------------------" << std::endl;

	handler.sendCgi();
	char line[100];
	while (fscanf(handler.getCgiResponse(), "%[^\n]", line) == 1)  
		std::cout << line << std::endl;
	delete handler._pgm_path;
	delete handler._script_path;
	return 0;
}
