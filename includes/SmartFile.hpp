#ifndef SMARTFILE_HPP
# define SMARTFILE_HPP

# include <string>
# include <iostream>
# include <cstdio>
#include "Server.hpp"

#define TMP_SUFFIX "-XXXXXX"
#define TMP_SUFFIX_LEN 7

class SmartFile
{
	public:
		SmartFile(void);
		SmartFile(std::string const &, std::string const &);
		SmartFile(SmartFile const &src);
		virtual	~SmartFile(void);
		SmartFile	&operator=(SmartFile const &rhs);

		int
		gets(char*, int) const;

		int
		puts(const char*, int);

		
	private:
		fd_t		_file;
		std::string _name;
		std::string _mode;
		char 		_tmpname[32];
};
#endif
