/*
 * this is used to listen and establish new connections.
 * by wareric@163.com
 * 2018-09-05
 */
#ifndef SERVER_LISTEN_H_H
#define SERVER_LISTEN_H_H

#define EXIT_LISTEN_ARGS		1		// unmatch input arguments
#define EXIT_LISTEN_INET_BIND		2		// inet init bind error
#define EXIT_LISTEN_UNIX_BIND		3		// inet init bind error

int init_listen_bind(int port);
int maxfd(int x, int y);

#endif
