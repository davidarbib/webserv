#include "CgiHandler.hpp"

#include <errno.h>
CgiHandler::CgiHandler(Request const &request, std::string const &pgm_path,
						std::string &script_path, std::string const &query)
: _pgm_path(pgm_path), _script_path(script_path), _query(query)
{
	//(void)request;
	request_line startline = request.getStartLine();
	//cutting URI in start line for env
	//addCgiEnv("AUTH_TYPE", "");
	addCgiEnv("CONTENT_LENGTH", request.get_header_value("Content-Length"));
	addCgiEnv("CONTENT_TYPE", request.get_header_value("Content-Type"));
	addCgiEnv("GATEWAY_INTERFACE", "CGI/1.1");
	addCgiEnv("PATH_INFO", _script_path);
	addCgiEnv("PATH_TRANSLATED", _script_path);
	addCgiEnv("QUERY_STRING", _query);
	//addCgiEnv("REMOTE_ADDR", "");
	//addCgiEnv("REMOTE_HOST", ""); 
	//addCgiEnv("REMOTE_IDENT", "");
	//addCgiEnv("REMOTE_USER", "");
	addCgiEnv("REQUEST_METHOD", startline.method_token);
	//addCgiEnv("REQUEST_METHOD", "POST");
	addCgiEnv("SCRIPT_NAME", _script_path);
	//addCgiEnv("SERVER_NAME", "");
	//addCgiEnv("SERVER_PORT", "");
	//addCgiEnv("SERVER_PROTOCOL", "");
	//addCgiEnv("SERVER_SOFTWARE", "");
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

/*
void
CgiHandler::sendCgi(void)
{
	int tocgi_pipe[2];
	int towebserv_pipe[2];
	char *argv[3];
	char *const *env = getCgiEnv();	

	argv[0] = const_cast<char*>(_pgm_path->c_str());
	argv[1] = const_cast<char*>(_script_path->c_str());
	argv[2] = NULL;

	__pipe__(tocgi_pipe);
	__pipe__(towebserv_pipe);
	pid_t pid = __fork__();	
	if (pid == 0)
	{
		__dup2__(tocgi_pipe[0], STDIN_FILENO);
		__dup2__(towebserv_pipe[1], STDOUT_FILENO);
		close(tocgi_pipe[1]);
		close(towebserv_pipe[0]);
		__execve__(_pgm_path->c_str(), argv, env);
	}
	else
	{
		__dup2__(tocgi_pipe[1], STDIN_FILENO);
		__dup2__(towebserv_pipe[0], STDOUT_FILENO);
		
		close(tocgi_pipe[0]);
		close(towebserv_pipe[1]);
	}
}
*/

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

//send env variables from request parser
//	
//send body by tmpfiles
//
//receive whole response by tmpfile : 
//	status
//	(CRLFCRLF)
//	body
//
//first milestone : test with php request / response
//
/*
 * meta variables contents are dquoted
 *
 * meta-variable-name = 
 * "AUTH_TYPE"  = "" (because auth is not handled by webserv)
 * "CONTENT_LENGTH" = from request eponym header or ""
 * "CONTENT_TYPE" = from request media-type
 * "GATEWAY_INTERFACE" = "CGI/1.1"
 * "PATH_INFO" = relative path for resource (e.g. php script path)
 * "PATH_TRANSLATED" = system URI for resource => absolute unix path
 * "QUERY_STRING" = case substring substring after the question mark char or ""
 * "REMOTE_ADDR" = client address
 * "REMOTE_HOST" = client hostname or "" 
 * "REMOTE_IDENT" = identity information optionnal
 * "REMOTE_USER" = user identification string for user authentification
 * 					(mandatory if AUTH_TYPE is "Basic" or "Digest")
 * "REQUEST_METHOD" = http method token 
 * "SCRIPT_NAME" = 
 * "SERVER_NAME" =
 * "SERVER_PORT" =
 * "SERVER_PROTOCOL" =
 * "SERVER_SOFTWARE" = 
 * + for php cgi : "REDIRECT_STATUS" = 200
*/

std::string const& CgiHandler::getPgm() const
{
	return _pgm_path;
}
