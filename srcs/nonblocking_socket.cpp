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

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	sockaddr_in sin;
	sockaddr_in new_sin;
	int			new_sock_fd;
	
	fd_set readfds;
	struct timeval tv;

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	bzero(reinterpret_cast<void*>(&sin), sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));

	listen(sock_fd, 1);
	socklen_t sinsize = sizeof(new_sin);
	
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	while(1)
	{
		//accept a new connection
		//for each connection
		//	read
		//	write
		new_sock_fd = accept(sock_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
		if (new_sock_fd > 0)
		{
			printf("New socket fd : %d\n", new_sock_fd);
			fcntl(new_sock_fd, F_SETFL, O_NONBLOCK);
			FD_SET(new_sock_fd, &readfds); 
		}
		int ret = select(1, &readfds, NULL, NULL, &tv);
		printf("ret : %d\n", ret);
		if (ret > 0)
		{
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
