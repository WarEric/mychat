#include<iostream>
#include<string>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include"client.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

ClientInfo *cliInfo;

bool closeTcp(ClientInfo *cli)
{
	if(cli->cliTcpFd >= 0)
	{
		close(cli->cliTcpFd);
		cout << "Tcp socket closed" << endl;
	}
	return true;
}

bool closeUdp(ClientInfo *cli)
{
	if(cliInfo->cliUdpFd >= 0)
	{
		close(cliInfo->cliUdpFd);
		cout << "Udp socket closed" << endl;
	}
	return true;
}

void signalHandler(int signum)
{
	cout << "we will exit from this chat" << endl;
	closeUdp(cliInfo);
	closeTcp(cliInfo);
	cout<< "exit success." << endl;
	exit(0);
}

//this an easy solution, we can make it better in another time.
bool login(ClientInfo *cli)
{
	cout << "login:";
	cin >> cli->name;
	cout << "passwd:";
	//not use now
	cout << endl;

	cout << "login successfully" << endl;

	return true;
}

bool tcpConnect(ClientInfo *cli)
{
	int n;
	struct sockaddr_in servaddr;

	cli->cliTcpFd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli->cliTcpFd < 0)
	{
		cout << "TCP socket error" << endl;
		return false;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(cli->servport);
	n = inet_pton(AF_INET, cli->servaddr.c_str(), &servaddr.sin_addr);
	if(n < 0){
		cout << "convert presentation " << cliInfo->servaddr << " to numeric wrong." << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << cliInfo->servaddr;
		return false;
	}

	//we can handle error type here
	if(connect(cli->cliTcpFd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "TCP connect error" << endl;
		return false;
	}
	
	return true;
}

bool udpConnect(ClientInfo *cli)
{
	int n;
	struct sockaddr_in cliaddr, servaddr;
	cli->cliUdpFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(cli->cliUdpFd < 0)
	{
		cout << "UDP socket error" << endl;
		return false;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(cli->servport);
	n = inet_pton(AF_INET, cli->servaddr.c_str(), &servaddr.sin_addr);
	if(n < 0){
		cout << "convert presentation " << cli->servaddr << " to numeric wrong." << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << cli->servaddr;
		return false;
	}

	if(connect(cli->cliUdpFd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "UDP connect error" << endl;
		return false;
	}
}

bool chat(ClientInfo *cli)
{
	while(1)
	{

	}
	return true;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "input format is wrong, example like this:\n"
			<< "./client <IPaddress>" << endl;
		exit(1);
	}
	
	cliInfo = new ClientInfo();
	cliInfo->servaddr = string(argv[1]);

	signal(SIGINT, signalHandler);

	if(tcpConnect(cliInfo) == false)
		exit(2);

	if(login(cliInfo) == false)
	{
		closeTcp(cliInfo);
		exit(3);
	}

	if(udpConnect(cliInfo) == false)
	{
		closeTcp(cliInfo);
		exit(4);
	}

	if(chat(cliInfo) == false)
	{
		closeUdp(cliInfo);
		closeTcp(cliInfo);
		exit(5);
	}

	return 0;
}
