#include<iostream>
#include<string>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<strings.h>
#include"ClientInfo.h"
#include"packet.h"
#include"write.h"
#include"read.h"
#define MAXLINE 4096

using std::cout;
using std::cin;
using std::endl;
using std::string;

ClientInfo *cliInfo;

int max(int x, int y)
{
	return x > y ? x : y ;
}

bool closeTcp(ClientInfo *cli)
{
	if(cli->tcpfd >= 0)
	{
		close(cli->tcpfd);
		cout << "Tcp socket closed" << endl;
	}
	return true;
}

bool closeUdp(ClientInfo *cli)
{
	if(cliInfo->udpfd >= 0)
	{
		close(cliInfo->udpfd);
		cout << "Udp socket closed" << endl;
	}
	return true;
}

void signalHandler(int signum)
{
	cout << "\nwe will exit from this chat" << endl;
	closeUdp(cliInfo);
	closeTcp(cliInfo);
	cout<< "exit success." << endl;
	exit(0);
}

//this an easy solution, we can make it better in another time.
bool login(ClientInfo *cli)
{

	string type("TCP");

	cout << "login:";
	cin >> cli->name;
	cout << "passwd:";
	cin >> cli->passwd;
	char buff[MAXLINE];

	LoginPacket pkt(cli->name, cli->passwd);
	int64_t datalen = encode_login_packet(pkt, buff, MAXLINE);

	Writen(cli->tcpfd, &datalen, sizeof(int64_t), type);
	Writen(cli->tcpfd, buff, datalen, type);

	cout << "login successfully" << endl;

	if(Read(cli->tcpfd, &datalen, sizeof(int64_t), string("TCP")) != sizeof(int64_t))
		return false;

	if(Read(cli->tcpfd, buff, datalen, string("TCP")) != datalen)
		return false;

	AuthResultPacket res;
	if(decode_auth_result_packet(res, buff) == false)
	{
		cout << "auth result decode error!" << endl;
		return false;
	}

	if(res.type != AUTH_RESULT_TYPE)
	{
		cout << "unmatch result type of login" << endl;
		return false;
	}

	if(res.result != 0)
	{
		cout << "login failure" << endl;
		return false;
	}

	cout << res.msg << endl;

	return true;
}

bool tcpConnect(ClientInfo *cli)
{
	int n;
	struct sockaddr_in servaddr;

	cli->tcpfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli->tcpfd < 0)
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
	if(connect(cli->tcpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "TCP connect error" << endl;
		return false;
	}
	
	return true;
}

bool udpConnect(ClientInfo *cli)
{
	cli->udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(cli->udpfd < 0)
	{
		cout << "UDP socket error" << endl;
		return false;
	}

	//get local tcp ipaddress and port, udp and tcp use the same ip and port
	int n;
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen = sizeof(cliaddr);

	if(getsockname(cli->tcpfd, (struct sockaddr *)&cliaddr, &clilen) == -1)
	{
                cout << "tcpfd(" << cli->tcpfd << ") can't getsockname." << endl;
		return false;
	}
	cli->cliaddr = string(inet_ntoa(cliaddr.sin_addr));
	cli->cliport = ntohs(cliaddr.sin_port);

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(cli->cliport);
	n = inet_pton(AF_INET, cli->cliaddr.c_str(), &cliaddr.sin_addr);
	if(n < 0){
		cout << "convert presentation" << cli->cliaddr << "to numeric wrong" << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << cli->cliaddr;
		return false;
	}

	if(bind(cli->udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "udp bind " << cli->cliaddr << ":" << cli->cliport
			<< "error" << endl;
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

	if(connect(cli->udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "UDP connect error" << endl;
		return false;
	}

	return true;
}

bool chat(ClientInfo *cli)
{
	int maxfd, n;
	fd_set rset;
	char buf[MAXLINE];

	FD_ZERO(&rset);
	while(1)
	{
		FD_SET(fileno(stdin), &rset);
		FD_SET(cli->udpfd, &rset);
		maxfd = max(fileno(stdin), cli->udpfd) + 1;

		if(select(maxfd, &rset, NULL, NULL, NULL) < 0)
		{
			cout << "select error" << endl;
			return false;
		}

		if(FD_ISSET(cli->udpfd, &rset)){
			if( (n = Read(cli->udpfd, buf, MAXLINE, string("UDP"))) == 0){
				cout << "server terminated" << endl;
				return false;
			}
		}
		Writen(fileno(stdout), buf, n, string("UDP"));

		if(FD_ISSET(fileno(stdin), &rset)){
			n = Read(fileno(stdin), buf, MAXLINE, string("Input UDP"));
			if(n < 0)
			{
				cout << "Read input error" << endl;
				return false;
			}
			Writen(cli->udpfd, buf, n, string("UDP"));
		}
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
