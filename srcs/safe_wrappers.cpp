#include "safe_wrappers.hpp"

pid_t
_fork_(void)
{
	pid_t pid = fork();
	if (pid < 0)
		throw ForkException();
	return pid;
}

pid_t
__fork__(void)
{
	try
	{
		return _fork_();
	}
	catch (ForkException &e)
	{
		std::cout << e.what() << std::endl;
		perror("");
	}
	return -1;
}

void
_pipe_(int *fds)
{
	int ret = pipe(fds);
	if (ret < 0)
		throw PipeException();
}

void
__pipe__(int *fds)
{
	try
	{
		_pipe_(fds);
	}
	catch (PipeException &e)
	{
		std::cout << e.what() << std::endl;
		perror("");
	}
}

void
_execve_(const char *pathname, char *const argv[], char *const envp[])
{
	if (execve(pathname, argv, envp) < 0)
		throw ExecveException();
}

void
__execve__(const char *pathname, char *const argv[], char *const envp[])
{
	try
	{
		_execve_(pathname, argv, envp);
	}
	catch (ExecveException &e)
	{
		std::cout << e.what() << std::endl;
		perror("execve");
	}
}

void
_dup2_(int oldfd, int newfd)
{
	int ret = dup2(oldfd, newfd);
	if (ret < 0)
		throw DupException();
}

void
__dup2__(int oldfd, int newfd)
{
	try
	{
		_dup2_(oldfd, newfd);
	}
	catch (DupException &e)
	{
		std::cout << e.what() << std::endl;
		perror("dup2");
	}
}

FILE *
_tmpfile_( void )
{
	FILE *file = tmpfile();
	if (!file)
		throw TmpFileException();
	return file;
}

FILE *
__tmpfile__( void )
{
	try
	{
		return _tmpfile_();
	}
	catch (TmpFileException &e)
	{
		std::cout << e.what() << std::endl;
		perror("tmpfile64");
	}
	return NULL;
}
