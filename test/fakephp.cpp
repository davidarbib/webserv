# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <iostream>
# include <fstream>

# define DELAY		1
# define BUFSIZE	1024
# define PORT		9000

typedef long fd_t;

int main()
{
	char			buf[BUFSIZE];
	sockaddr_in		sin;
	sockaddr_in		new_sin;
	fd_t			new_sock_fd;
	socklen_t		sinsize = sizeof(new_sin);
	fd_t			listen_fd;
	std::ofstream	cgi_log;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1)
	{
		perror("fakephp");
		exit(EXIT_FAILURE);
	}
/*
	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fakephp");
		exit(EXIT_FAILURE);
	}
*/

	memset(reinterpret_cast<void*>(&sin), 0, sizeof(sockaddr_in));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if (bind(listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if (listen(listen_fd, 1) == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	new_sock_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&new_sin), &sinsize);
	if (new_sock_fd < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	cgi_log.open("cgi_log");
	memset(buf, 0, BUFSIZE);
	recv(new_sock_fd, buf, BUFSIZE, 0);
	std::cout << buf;
	//int recvret = 1;
	//while (recvret > 0)
	//{
	//	std::cout << "test";
	//	cgi_log << "test";
	//	memset(buf, 0, BUFSIZE);
	//	recvret = recv(new_sock_fd, buf, BUFSIZE, 0);
	//	buf[0] = 'a';
	//	std::cout << buf;
	//	cgi_log << buf;
	//}
	cgi_log.close();
	return 0;
}
