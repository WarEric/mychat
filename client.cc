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

bool tcpConnect(ClientInfo *cli);
bool udpConnect(ClientInfo *cli);

bool login(ClientInfo *cli);
bool inputPassword(string &str);
bool chat(ClientInfo *cli);

bool closeTcp(ClientInfo *cli);
bool closeUdp(ClientInfo *cli);

void signalHandler(int signum);

int max(int x, int y);

ClientInfo *cliInfo;
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

	if(udpConnect(cliInfo) == false)
	{
		closeTcp(cliInfo);
		exit(3);
	}

	if(login(cliInfo) == false)
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
	int n;
	cli->udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(cli->udpfd < 0)
	{
		cout << "UDP socket error" << endl;
		return false;
	}

	struct sockaddr_in servaddr;
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

	//get local udp ipaddress and port
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	if(getsockname(cli->udpfd, (struct sockaddr *)&cliaddr, &len) == -1)
	{
                cout << "udpfd(" << cli->udpfd << ") can't getsockname." << endl;
		return false;
	}
	cli->cliaddr = string(inet_ntoa(cliaddr.sin_addr));
	cli->cliport = ntohs(cliaddr.sin_port);

	return true;
}

//this an easy solution, we can make it better in another time.
bool login(ClientInfo *cli)
{
	AuthResultPacket res;
	int count;
	for(int count = 0; count < 5; count++)
	{
		do{
			cout << "login:";
			cin >> cli->name;
		}while(inputPassword(cli->passwd) != true);

		char buff[MAXLINE];

		LoginPacket pkt(cli->name, cli->passwd, cli->cliaddr, cli->cliport);
		int64_t datalen = encode_login_packet(pkt, buff, MAXLINE);

		Writen(cli->tcpfd, &datalen, sizeof(int64_t), string("tcp"));
		Writen(cli->tcpfd, buff, datalen, string("tcp"));


		if(Read(cli->tcpfd, &datalen, sizeof(int64_t), string("tcp")) != sizeof(int64_t))
			return false;
		if(Read(cli->tcpfd, buff, datalen, string("tcp")) != datalen)
			return false;

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

		if(res.result == 0)
			break;

		cout << "login failure." << endl;
		cout << res.msg << endl;
	}
	if(count == 5)
		return false;

	cout << "login successfully." << endl;
	cout << res.msg << endl;

	return true;
}

//This is not the best way. System man recommend termios as a better way
bool inputPassword(string &str)
{
	char *passwd = getpass("passwd:");
	str = string(passwd);

	if(str.empty())
		return false;
	else
		return true;
}

bool chat(ClientInfo *cli)
{
	int maxfd, n;
	fd_set rset;
	char buf[MAXLINE];

	while(1)
	{

		FD_ZERO(&rset);
		FD_SET(fileno(stdin), &rset);
		FD_SET(cli->udpfd, &rset);
		FD_SET(cli->tcpfd, &rset);

		maxfd = max(cli->tcpfd, cli->udpfd) + 1;
		maxfd = max(maxfd, fileno(stdin));

		if(select(maxfd, &rset, NULL, NULL, NULL) < 0)
		{
			cout << "select error" << endl;
			return false;
		}

		if(FD_ISSET(cli->udpfd, &rset)){
			if( (n = read(cli->udpfd, buf, MAXLINE)) == 0){
				cout << "server terminated" << endl;
				return false;
			}
			write(fileno(stdout), buf, n);
		}

		if(FD_ISSET(fileno(stdin), &rset)){
			if( (n = read(fileno(stdin), buf, MAXLINE)) < 0)
			{
				cout << "Read input error" << endl;
				return false;
			}
			write(cli->udpfd, buf, n);
		}

		if(FD_ISSET(cli->tcpfd, &rset)){
			if((n = read(cli->tcpfd, buf, MAXLINE)) <= 0)
			{
				if(n == 0)
				{
					cout << "remote server tcp connection shutdown, we will exit now" << endl;
					return false;
				}
				else{
					cout << "read tcp error" << endl;
					return false;
				}
			}
		}
	}
	return true;
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

int max(int x, int y)
{
	return x > y ? x : y ;
}
