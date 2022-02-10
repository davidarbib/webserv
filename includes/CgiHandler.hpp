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
# include "Server.hpp"
# include "safe_wrappers.hpp"
#include <sys/types.h>
#include <sys/wait.h>


class CgiHandler
{
	public:
		CgiHandler(void);
		//CgiHandler(Request &request);
		//TODO remove second and third arg
		CgiHandler(Request &request, std::string* pgm_path, std::string *script_path);
		virtual	~CgiHandler(void);
		
		void
		sendCgi(void);

		void
		buildCgi(void);

		char **
		getCgiEnv(void); //TODO go private 

		FILE *
		getCgiResponse(void); //TODO go private 

		FILE *
		getCgiRequest(void); //TODO delete 

		std::string							*_pgm_path; //TODO go private
		std::string							*_script_path; //TODO go private

	private:
		CgiHandler(CgiHandler const &src);
		CgiHandler	&operator=(CgiHandler const &rhs);

		void
		addCgiEnv(std::string const &, std::string const &);
		
		std::string							_request;
		std::map<std::string, std::string>	_cgi_env;
		FILE								*_sender;
		FILE								*_receiver;
};
#endif
