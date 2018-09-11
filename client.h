/*
 * clients default configuration.
 */
#ifndef CLIENT_H_H
#define CLIENT_H_H

#include <iostream>
#include "Code.h"
#include "Passwd.h"

#define EXIT_CLIENT_INPUT_ARG_WRONG		1		// unmatch input arguments
#define EXIT_CLIENT_LOGIN_FAILURE		2		// login failure

#define LOGIN_TRY_MAX_NUMBER			5		// login biggest try times.

#define SERVER_LOGIN_PORT			63353
#define MAXLINE					4096


bool login(Code* code, Passwd* passwd, char *servaddr, int servport);

int udpsocket();
bool udpconnect(int fd, string addr, int port);
bool success_login_connect(int chatfd, int chatport, int heartfd, int heartport, std::string addr);

int getsockport(int fd);
string getsockaddr(int fd);

void clearfd_exit(int num);
void signalHandler(int signum);
#endif
