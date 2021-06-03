#include "Request.hpp"
#include <iostream>
#include <fstream>

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
			s_request += '\n';
		}
	}
	else
		return "";
	return s_request;
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		std::string raw_request = parse_raw_request(av[1]);
		std::cout << raw_request;
		return 0;
	}
	else
	{
		std::cerr << "Error : bad argument numbers" << std::endl;
		return 1;
	}
}