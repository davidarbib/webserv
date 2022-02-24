#include "CgiHandler.hpp"

#include <errno.h>
CgiHandler::CgiHandler(Request const &request, std::string const &pgm_path,
						std::string &script_path, std::string const &query)
: _pgm_path(pgm_path), _script_path(script_path), _query(query)
{
	request_line startline = request.getStartLine();
	addCgiEnv("CONTENT_LENGTH", request.getHeaderValue("Content-Length"));
	addCgiEnv("CONTENT_TYPE", request.getHeaderValue("Content-Type"));
	addCgiEnv("GATEWAY_INTERFACE", "CGI/1.1");
	addCgiEnv("PATH_INFO", _script_path);
	addCgiEnv("PATH_TRANSLATED", _script_path);
	addCgiEnv("QUERY_STRING", _query);
	addCgiEnv("REQUEST_METHOD", startline.method_token);
	addCgiEnv("SCRIPT_NAME", _script_path);
	addCgiEnv("REDIRECT_STATUS", "200");

	_sender = __tmpfile__();
	_receiver = __tmpfile__();
	
	std::string body;
	body.assign(request.getBody().begin(), request.getBody().end());
	write(fileno(_sender), body.c_str(), body.size()); //TODO Exception handling , and fd select checking
	rewind(_sender);
}

CgiHandler::~CgiHandler(void)
{
	fclose(_sender);
	fclose(_receiver);
}

void
CgiHandler::addCgiEnv(const std::string &var_name, const std::string &value)
{
	_cgi_env[var_name] = value;
}

char **
CgiHandler::getCgiEnv(void)
{
	char **env = NULL;

	try
	{
		env = new char*[_cgi_env.size() + 1];
	}
	catch (std::bad_alloc &e)
	{
		std::cout << e.what() << std::endl;
	}

	int i = 0;
	for (std::map<std::string, std::string>::iterator mit = _cgi_env.begin();
			mit != _cgi_env.end();
			mit++, i++)
	{
		std::string concat = mit->first + "=" + mit->second;
		try
		{
			env[i] = new char[concat.size() + 1];
		}
		catch(std::bad_alloc &e)
		{
			for (int j = i - 1; j <= 0; j--)
				delete env[i];
			delete env;
			std::cout << e.what() << std::endl;
		}
		strcpy(env[i], concat.c_str());
	}
	env[i] = NULL;
	return env;
}

void
CgiHandler::sendCgi(void)
{

	char *argv[3];
	char *const *env = getCgiEnv();	

	argv[0] = const_cast<char*>(_pgm_path.c_str());
	argv[1] = const_cast<char*>(_script_path.c_str());
	argv[2] = NULL;

	pid_t pid = __fork__();	
	if (pid == 0)
	{
		__dup2__(fileno(_sender), STDIN_FILENO);
		__dup2__(fileno(_receiver), STDOUT_FILENO);
		__execve__(_pgm_path.c_str(), argv, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	rewind(_receiver);
}

FILE *
CgiHandler::getCgiResponse(void)
{
	return _receiver;
}

std::string const& CgiHandler::getPgm() const
{
	return _pgm_path;
}
