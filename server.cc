#include<iostream>
#include<string.h>
#include<string>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include<strings.h>
#include<arpa/inet.h>
#include<strings.h>
#include"server.h"
#include"packet.h"
#include"write.h"
#include"read.h"
#include"ClientInfo.h"

using std::cout;
using std::endl;
using std::string;

bool udpConnect(ClientInfo *cli)
{
	int n;
	struct sockaddr_in cliaddr, servaddr;
	
	cli->udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(cli->udpfd < 0)
	{
		cout << "UDP socket error" << endl;
		return false;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(cli->servport);
	n = inet_pton(AF_INET, cli->servaddr.c_str(), &servaddr.sin_addr);
	if(n < 0){
		cout << "convert presentation" << cli->servaddr << " to numeric wrong" << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << cli->servaddr;
		return false;
	}

	if(bind(cli->udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "udp bind " << cli->servaddr << ":" << cli->servport
			<< " error" << endl;
		return false;
	}

	bzero(&cliaddr, sizeof(servaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(cli->cliport);
	n = inet_pton(AF_INET, cli->cliaddr.c_str(), &cliaddr.sin_addr);
	if(n < 0){
		cout << "convert presentation" << cli->cliaddr << " to numeric wrong" << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << cli->cliaddr;
		return false;
	}

	if(connect(cli->udpfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
	{
		cout << "udp connect " << cli->cliaddr << ":" 
			<< cli->cliport << " error" << endl;
		return false;
	}
}

bool getClientInfo(ClientInfo *cli, int sockfd, char buff[], int64_t datalen)
{
	LoginPacket pkt;
	if(decode_login_packet(pkt, buff) == false)
	{
		cout << "login packet decode error! " <<  endl;
		return false;
	}

	//get account
	cli->name = pkt.name;
	cli->passwd = pkt.passwd;
	cli->tcpfd = sockfd;

	//getlocalinfo
	struct sockaddr_in localaddr;
	socklen_t locallen = sizeof(localaddr);

	if(getsockname(sockfd, (struct sockaddr *)&localaddr, &locallen) == -1)
	{
		cout << "sockfd(" << sockfd << ") can't getsockname." << endl;
		return false;
	}

	cli->servaddr = string(inet_ntoa(localaddr.sin_addr));
	cli->servport = ntohs(localaddr.sin_port);

	//getpeerinfo
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	if(getpeername(sockfd, (struct sockaddr *)&peeraddr, &peerlen) == -1)
	{
		cout << "sockfd(" << sockfd << ") can't getsockname." << endl;
		return false;
	}

	cli->cliaddr = string(inet_ntoa(peeraddr.sin_addr));
	cli->cliport = ntohs(peeraddr.sin_port);

	return true;
}

bool login(int sockfd, char buff[], int64_t datalen)
{
	ClientInfo cli;
	if(getClientInfo(&cli, sockfd, buff, datalen) == false)
	{
		AuthResultPacket res;
		res.result = 1;
		res.msg = string("\nserver can't get ClientInfo !!! \n");

		char tempbuf[MAXLINE];
		int64_t templen = encode_auth_result_packet(res, tempbuf, MAXLINE);
		Writen(sockfd, &templen, sizeof(int64_t), string("TCP"));
		Writen(sockfd, tempbuf, templen, string("TCP"));

		cout << "login get ClientInfo error." << endl;
		return false;
	}

	//check operation
	//
	//
	//
	
	if(udpConnect(&cli) == false)
	{
		AuthResultPacket res;
		res.result = 1;
		res.msg = string("\nserver can't establish udp channel !!! \n");

		char tempbuf[MAXLINE];
		int64_t templen = encode_auth_result_packet(res, tempbuf, MAXLINE);
		Writen(sockfd, &templen, sizeof(int64_t), string("TCP"));
		Writen(sockfd, tempbuf, templen, string("TCP"));
		
		cout << "establish udp connection to " << cli.name << " fail" << endl;
		return false;
	}
	
	cout << cli.name << " join the chat." << endl;

	AuthResultPacket res;
	res.result = 0;
	res.msg = string("\nWelcome join the chat !!! \n");

	char tempbuf[MAXLINE];
	int64_t templen = encode_auth_result_packet(res, tempbuf, MAXLINE);
	Writen(sockfd, &templen, sizeof(int64_t), string("TCP"));
	Writen(sockfd, tempbuf, templen, string("TCP"));

	return true;
}

int init_listen()
{
	int listenfd;
	struct sockaddr_in servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "TCP listenfd bind error" << endl;
		exit(1);
	}

	if(listen(listenfd, LISTENQ) < 0)
	{
		cout << "TCP listenfd listen error" << endl;
		exit(2);
	}
	return listenfd;
}

int tcp_listen_event(int listenfd, fd_set *rset, fd_set *allset, int tcp[], int* tcp_maxi, int nready)
{
	int i, connfd;
	socklen_t clilen;
	struct sockaddr_in cliaddr;

	if(FD_ISSET(listenfd, rset)){
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		if(connfd < 0){
			cout << "accept error" << endl;
			exit(3);
		}

		for(i = 0; i < FD_SETSIZE; i++)
			if(tcp[i] < 0){
				tcp[i] = connfd;
				break;
			}
		if(i == FD_SETSIZE){
			cout << "too many clients" << endl;
			exit(3);
		}

		FD_SET(connfd, allset);

		if(i > *tcp_maxi)
			*tcp_maxi = i;

		--nready;
	}

	return nready;
}

int tcp_event(int tcp[], int maxi, fd_set *rset, fd_set *allset, int nready)
{
	int sockfd;
	char buf[MAXLINE];

	uint8_t type;
	int64_t datalen;
	int size = sizeof(int64_t);

	for(int i = 0; i <= maxi; i++){
		if((sockfd = tcp[i]) < 0)
			continue;
		if(FD_ISSET(sockfd, rset)){
			if(Read(sockfd, &datalen, size, string("TCP")) != size){
				close(sockfd);
				FD_CLR(sockfd, allset);
				tcp[i] = -1;
				cout << "some one left the chat !!!" << endl;
			}else{
				if(Read(sockfd, buf, datalen, string("TCP")) == datalen)
				{
					memcpy(&type, buf, 1);
					switch(type)
					{
						case LOGIN_TYPE:
							login(sockfd, buf, datalen);
							break;
						default:
							cout << "TCP unrecognised packet type" << endl;
							break;
					}
				}else{
					close(sockfd);
					FD_CLR(sockfd, allset);
					tcp[i] = -1;
				}
			}

			if(--nready <= 0)
				break;
		}
	}
	return nready;
}

int maxfd(int tcpfd[], int tcp_maxi, int udpfd[], int udp_maxi)
{
	int max = -1;
	for(int i = 0; i < tcp_maxi; i++)
	{
		if(tcpfd[i] > max)
			max = tcpfd[i];
	}

	for(int i = 0; i < udp_maxi; i++)
	{
		if(udpfd[i] > max)
			max = udpfd[i];
	}
	return max;
}

int main(int argc, char *argv[])
{
	int tcp_maxi, udp_maxi, listenfd;
	int nready, cliTcp[FD_SETSIZE], cliUdp[FD_SETSIZE];

	fd_set rset, allset;

	listenfd = init_listen();

	for(int i = 0; i < FD_SETSIZE; i++)
	{
		cliTcp[i] = -1;
		cliUdp[i] = -1;
	}
	
	tcp_maxi = -1;
	udp_maxi = -1;

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	
	while(1){
		rset = allset;
		nready = select(maxfd(cliTcp, tcp_maxi, cliUdp, udp_maxi)+1, &rset, NULL, NULL, NULL);

		nready = tcp_listen_event(listenfd, &rset, &allset, cliTcp, &tcp_maxi, nready);
		nready = tcp_event(cliTcp, tcp_maxi, &rset, &allset, nready);
	}

}
