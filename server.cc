/**
 * This a server functions library
 * by wareric@163.com
 * 2018-09-05
 * */
#include "server.h"

int init_unix_bind(char *path)
{
	int sockfd, res;
	struct sockaddr_un servaddr, cliaddr;

	sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		cout << "unix " << string(path) << " socket error" << endl;
		return sockfd;
	}

	unlink(path);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, path);

	res = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(res < 0)
	{
		cout << "unix " << string(path) << " bind error" << endl;
		return res;
	}
	return sockfd;
}
