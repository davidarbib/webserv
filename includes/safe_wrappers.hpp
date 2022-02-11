#ifndef SAFE_WRAPPERS_HPP
# define SAFE_WRAPPERS_HPP

# include <string>
# include <iostream>
# include <sys/types.h>
# include <unistd.h>
# include <cstdio>
# include <errno.h>
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

class DupException : public std::exception
{
	public :
		virtual const char*
		what() const
		throw()
		{ return "Error while duplicate fd"; }
};

class TmpFileException : public std::exception
{
	public :
		virtual const char*
		what() const
		throw()
		{ return "Error when create tmp file"; }
};

int
_fork_(void);

int
__fork__(void);

void
_pipe_(int *fds);

void
__pipe__(int *fds);

void
_execve_(const char *pathname, char *const argv[], char *const envp[]);

void
__execve__(const char *pathname, char *const argv[], char *const envp[]);

void
_dup2_(int oldfd, int newfd);

void
__dup2__(int oldfd, int newfd);

FILE *
_tmpfile_( void );

FILE *
__tmpfile__( void );


#endif
