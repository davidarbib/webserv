#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <iostream>
# include <algorithm>
# include <cstdio>
# include <cstring>
# include <unistd.h>
# include <map>
# include "Request.hpp"
# include ""
# include "Server.hpp"
# include "safe_wrappers.hpp"
#include <sys/types.h>
#include <sys/wait.h>


class CgiHandler
{
	public:
		CgiHandler(void);
		CgiHandler(CgiHandler const &src);
		CgiHandler(Request const &, std::string const &, std::string &, std::string const &);
		virtual	~CgiHandler(void);
		
		void
		sendCgi(void);

		void
		buildCgi(void);

		FILE *
		getCgiResponse(void);

		std::string const& getPgm() const;


	private:
		std::string	const					&_pgm_path; 
		std::string							&_script_path;
		std::string const 					&_query;

		CgiHandler	&operator=(CgiHandler const &rhs);

		void
		addCgiEnv(std::string const &, std::string const &);
		
		std::string							_request;
		std::map<std::string, std::string>	_cgi_env;
		FILE								*_sender;
		FILE								*_receiver;
		
		char **
		getCgiEnv(void);
};
#endif
