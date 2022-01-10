#include "CgiHandler.hpp"
#include "Request.hpp"
#include <iostream>

int main()
{
	Request request;

	request.set_method_token("GET");
	request.set_request_URI("www.example.com/test?querystring");
	request.set_http_version("1.1");
	request.set_body("cc");
	request.set_header("Content-Length", "2");
	request.set_header("Content-Type", "text/html");
	CgiHandler handler(request);

	char **cgi_env = handler.getCgiEnv();
	int i = 0;
	while (cgi_env[i])
		std::cout << cgi_env[i++] << std::endl;
	int j = 0;
	while (cgi_env[j])
		delete [] cgi_env[j++];
	delete [] cgi_env;
	return 0;
}
