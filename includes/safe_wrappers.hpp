#ifndef SAFE_WRAPPERS_HPP
# define SAFE_WRAPPERS_HPP

# include <string>
# include <iostream>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h> 

class ForkException : public std::exception
{
	public :
		virtual const char*
		what() const
		throw()
		{ return "Error when forking for CGI"; }
};

class PipeException : public std::exception
{
	public :
		virtual const char*
		what() const
		throw()
		{ return "Error while pipe creation"; }
};

class ExecveException : public std::exception
{
	public :
		virtual const char*
		what() const
		throw()
		{ return "Error while executing CGI interpreter"; }
};

int
_fork_(void);

void
_pipe_(int *fds);

void
_execve_(const char *pathname, char *const argv[], char *const envp[]);


#endif
