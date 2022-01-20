#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <iostream>
# include <algorithm>
# include <cstdio>
# include <map>
# include "Request.hpp"

# define QUERYCHAR '?'

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

		std::string
		extractQuery(std::string); //TODO go private

	private:
		CgiHandler(CgiHandler const &src);
		CgiHandler	&operator=(CgiHandler const &rhs);

		char **
		getCgiEnv(void);
		
		void
		addCgiEnv(const std::string&, const std::string&);

		std::string							_request;
		std::map<std::string, std::string>	_cgi_env;
		FILE								*_sender;
		FILE								*_receiver;
};
#endif
