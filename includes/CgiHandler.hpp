#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <iostream>
# include <cstdio>
# include <map>
# include <Request.hpp>

class CgiHandler
{
	public:
		CgiHandler(void);
		CgiHandler(Request &request);
		virtual	~CgiHandler(void);
		
		void
		sendCgi(void);

		void
		buildCgi(void);

	private:
		CgiHandler(CgiHandler const &src);
		CgiHandler	&operator=(CgiHandler const &rhs);

		char **
		getCgiEnv(void);

		std::string							_request;
		std::map<std::string, std::string>	_cgi_env;
};
#endif
