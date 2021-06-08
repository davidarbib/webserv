#include "Request.hpp"
#include <iostream>
#include <fstream>

void	parse_request(char *raw_request, Request *request);

std::string parse_raw_request(char *source)
{
	std::ifstream 	raw_request(source);
	std::string		s_request;
	std::string		line;
	if (raw_request)
	{
		while (getline(raw_request, line))
		{
			s_request += line;
			s_request += "\r\n";
		}
	}
	else
		return "";
	return s_request;
}

int main(int ac, char **av)
{
	Request request;
	if (ac == 2)
	{
		std::string raw_request = parse_raw_request(av[1]);
		if (raw_request.empty())
		{
			std::cerr << "Error: can't open the request file" << std::endl;
			return 1;
		}
		std::cout << raw_request;
		char *s_request = const_cast<char *>(raw_request.c_str());
		parse_request(s_request, &request);
		std::cout << request << std::endl;
		return 0;
	}
	else
	{
		std::cerr << "Error : bad argument numbers" << std::endl;
		return 1;
	}
}