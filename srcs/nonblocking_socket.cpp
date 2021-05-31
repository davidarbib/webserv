#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/select.h>

#define PORT	80
#define BUFSIZE 2000
#define DELAY	5

int	create_connection()
{
	sockaddr_in new_sin;
	int			new_sock_fd;
	socklen_t sinsize = sizeof(new_sin);

	new_sock_fd = accept(sock_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	printf("New socket fd : %d\n", new_sock_fd);
	fcntl(new_sock_fd, F_SETFL, O_NONBLOCK);
	FD_SET(new_sock_fd, &readfds); 
	FD_SET(new_sock_fd, &writefds); 
	if (new_sock_fd > maxfd)
		maxfd = new_sock_fd;
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	sockaddr_in sin;
	int			maxfd;
	
	fd_set readfds;
	struct timeval tv;

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	maxfd = sockfd;

	tv.tv_sec = DELAY;
	tv.tv_usec = 0;
	bzero(reinterpret_cast<void*>(&sin), sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
	listen(sock_fd, 1);
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	FD_SET(sock_fd, &readfs);
	while (1)
	{
		//select
		//check listen socket
		//	connection creation
		//		
		int ret = select(maxfd + 1, &readfds, &writefds, NULL, &tv);
		if (FD_ISSET(sock_fd, &readfs))
			create_connection();
		if (new_sock_fd > 0)
		{
			continue;
		}
		if (!ret)
			printf("No data within %d seconds\n", DELAY);
		if (ret > 0)
		{
			printf("read socket : %d\n", new_sock_fd);
			char buf[BUFSIZE];
			bzero(buf, BUFSIZE);
			read(new_sock_fd, buf, BUFSIZE);
			printf("%s\n", buf);
		}
		if (new_sock_fd > 0)
			close(new_sock_fd);
	}
	close(sock_fd);
	return 0;
}
