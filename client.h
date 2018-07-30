#ifndef CLIENT_H_
#define CLIENT_H_
#include<string.h>
using std::string;

#define SERVPORT 5678
class ClientInfo{
	public:
		ClientInfo():servport(SERVPORT), cliUdpFd(-1), cliTcpFd(-1){}
		ClientInfo(string nickname):name(nickname),servport(SERVPORT), cliUdpFd(-1), cliTcpFd(-1){}

		string name;
		string servaddr;
		int servport;
		int cliUdpFd;
		int cliTcpFd;
};
#endif
