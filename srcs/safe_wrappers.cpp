#include "safe_wrappers.hpp"

pid_t
_fork_(void)
{
	pid_t pid = fork();
	if (pid < 0)
		throw ForkException();
	return pid;
}

void
_pipe_(int *fds)
{
	int ret = pipe(fds);
	if (ret < 0)
		throw PipeException();
}

void
_execve_(const char *pathname, char *const argv[], char *const envp[])
{
	if (execve(pathname, argv, envp) < 0)
		throw ExecveException();
}
