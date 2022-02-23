#ifndef SMARTFILE_HPP
# define SMARTFILE_HPP

# include <string>
# include <iostream>
# include <cstdio>
#include "Server.hpp"

class SmartFile
{
	public:
		SmartFile(void);
		SmartFile(std::string const &, std::string const &);
		SmartFile(SmartFile const &src);
		virtual	~SmartFile(void);
		SmartFile	&operator=(SmartFile const &rhs);

		char *
		read(char*, int);

		int
		write(const char*);

		
	private:
		FILE		*_file;
		std::string _name;
		std::string _mode;
};
#endif
