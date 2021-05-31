#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

#define PORT	80
#define BUFSIZE 2000

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sockaddr_in new_sin;

	bzero(reinterpret_cast<void*>(&sin), sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	bind(sock_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));

	listen(sock_fd, 1);
	socklen_t sinsize = sizeof(new_sin);
	
	int new_sock_fd = accept(sock_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	char buf[BUFSIZE];
	bzero(buf, BUFSIZE);
	read(new_sock_fd, buf, BUFSIZE);
	printf("%s\n", buf);
	close(new_sock_fd);
	close(sock_fd);

	return 0;
}
