/**
 * this is used for listening and establishing new connections.
 * by wareric@163.com
 * 2018-08-17
 **/
#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include "server.h"
#include "server_listen.h"

using std::cout;
using std::cin;
using std::endl;

/*
 * input arguments: port, listenpath, heartpath, chatpath
 */
int main(int argc, char *argv[])
{

	int listenfd, unixfd;
	int nready;

	if(argc < 5)
	{
		cout << "input should be: " << string(argv[0]) 
			<< " port listenpath heartpath chatpath" << endl;
		exit(EXIT_LISTEN_ARGS);
	}
	if( (listenfd = init_listen_bind(atoi(argv[1]))) < 0)
		exit(EXIT_LISTEN_INET_BIND);

	if( (unixfd = init_unix_bind(argv[2])) < 0)
		exit(EXIT_LISTEN_UNIX_BIND);

	fd_set rset;
	FD_ZERO(&allset);
	FD_SET(listenfd, &rset);
	FD_SET(unixfd, &rset);
	int maxfd = maxfd(listenfd, unixfd);
	while(1){
		nready = select(maxfd, &rset, NULL, NULL, NULL);
		// handle listenfd
		// handle unixfd
	}
}

/**
 * return < 0, error
 * return >= 0, success
 *  */
int init_listen_bind(int port)
{
	int res, sockfd;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		cout << "udp socket error" << endl;
		return sockfd;
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	res = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0;
	if(res < 0)
	{
		cout << "udp bind error" << endl;
		return res;
	}
	return sockfd;
}

int maxfd(int x, int y)
{
	return x > y ? x : y;
}
