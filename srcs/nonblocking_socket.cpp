#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <csignal>

#include <fcntl.h>
#include <sys/select.h>

#define PORT	8003
#define BUFSIZE 2000
#define DELAY	5

int g_close_flag = 0;

void signalHandler(int signum)
{
	g_close_flag = 1;
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	exit(1);
}

int	create_connection(int sock_fd, fd_set *origin_fds, int *maxfd)
{
	sockaddr_in new_sin;
	int			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

	new_sock_fd = accept(sock_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	if (new_sock_fd < 0)
		perror("socket");
	printf("New socket fd : %d\n", new_sock_fd);
	fcntl(new_sock_fd, F_SETFL, O_NONBLOCK);
	FD_SET(new_sock_fd, origin_fds);
	if (new_sock_fd > *maxfd)
		*maxfd = new_sock_fd;
	return new_sock_fd;
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	sockaddr_in sin;
	int			maxfd;
	std::vector<int> fds;
	
	fd_set read_fds;
	fd_set write_fds;
	fd_set origin_fds;
	struct timeval tv;

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	std::cout << "sock fd at beginning : " << sock_fd << std::endl;
	maxfd = sock_fd;

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;
	bzero(reinterpret_cast<void*>(&sin), sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	int retbind = bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
	if (retbind < 0)
	{
		perror("webserv");
		exit(EXIT_FAILURE);
	}
	listen(sock_fd, 1);
	int writeyet = 0;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	signal(SIGINT, signalHandler);
	while (1)
	{
		FD_SET(sock_fd, &read_fds);
		//std::cout << read_fds.fds_bits[0] << std::endl;
		if (fds.size() > 0)
		{
			FD_SET(fds[0], &read_fds);
			FD_SET(fds[0], &write_fds);
		}
		int ret = select(maxfd + 1, &read_fds, &write_fds, NULL, &tv);
		if (FD_ISSET(sock_fd, &read_fds) && !g_close_flag)
		{
			if (recv(sock_fd, buf, BUFSIZE, NULL) > 0)
				std::cout << "on listen socket" << std::endl;
			std::cout << "connection" << std::endl;
			fds.push_back(create_connection(sock_fd, &origin_fds, &maxfd));	
			FD_CLR(sock_fd, &read_fds);
		}
		/*
		if (ret)
			// reads / writes
		*/
		if (fds.size() > 0 && ret > 0)
		{
			if (FD_ISSET(fds[0], &read_fds) && !g_close_flag)
			{
				//printf("read socket : %d\n", fds[0]);
				char buf[BUFSIZE];
				bzero(buf, BUFSIZE);
				int readret = read(fds[0], buf, BUFSIZE);
				printf("readret : %d\n", readret);
				if (readret == 0)
				{
					close(fds[0]);
					fds.pop_back();
				}
				printf("%s\n", buf);
				FD_CLR(fds[0], &read_fds);
			}
			if (FD_ISSET(fds[0], &write_fds) && !writeyet)
			{
				printf("write in socket : %d\n", fds[0]);
				write(fds[0], "coucou\n", 7);
				FD_CLR(fds[0], &write_fds);
				writeyet = 1;
			}
		}
		if (g_close_flag)
			break;
	}
	//close all sockets
	if (fds.size() > 0)
		close(fds[0]);
	std::cout << "sock fd at close: " << sock_fd << std::endl;
	close(sock_fd);
	return 0;
}
