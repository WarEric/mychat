#include<iostream>
#include<map>
#include<string.h>
#include<stdio.h>
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
using std::map;
using std::make_pair;

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

	return true;
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

bool login(int sockfd, map<int, ClientInfo> &tcpfdcli, map<int, ClientInfo> &udpfdcli, char buff[], int64_t datalen, fd_set *allset)
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
		res.result = 2;
		res.msg = string("\nserver can't establish udp channel !!! \n");

		char tempbuf[MAXLINE];
		int64_t templen = encode_auth_result_packet(res, tempbuf, MAXLINE);
		Writen(sockfd, &templen, sizeof(int64_t), string("TCP"));
		Writen(sockfd, tempbuf, templen, string("TCP"));
		
		cout << "establish udp connection to " << cli.name << " fail" << endl;
		return false;
	}

	FD_SET(cli.udpfd, allset);
	udpfdcli.insert(make_pair(cli.udpfd, cli));
	auto it = tcpfdcli.find(cli.tcpfd);
	it->second = cli;
	
	AuthResultPacket res;
	res.result = 0;
	res.msg = string("\nWelcome join the chat !!! \n");

	char tempbuf[MAXLINE];
	int64_t templen = encode_auth_result_packet(res, tempbuf, MAXLINE);
	Writen(sockfd, &templen, sizeof(int64_t), string("TCP"));
	Writen(sockfd, tempbuf, templen, string("TCP"));

	cout << cli.name << " join the chat." << endl;

	return true;
}

void chatall(int sockfd, map<int, ClientInfo> &udpfdcli, char buf[])
{
	cout << "char all" << endl;
	int n;
	char msg[MAXLINE];

	auto speaker = udpfdcli.find(sockfd);
	n = snprintf(msg, MAXLINE, "%s: %s", speaker->second.name.c_str(), buf);
	if(n > MAXLINE)
	{
		cout << "snprintf wrong" << endl;
		return;
	}

	for(auto it = udpfdcli.begin(), end = udpfdcli.end(); it != end; it++)
	{
		if(it == speaker)
			continue;

		Writen(it->second.udpfd, msg, n, string("UDP"));
	}
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

	cout << "Tcp listenning ......" << endl;
	return listenfd;
}

int tcp_listen_event(int listenfd, map<int, ClientInfo> &tcpfdcli, fd_set *rset, fd_set *allset, int nready)
{
	int i, connfd;

	if(FD_ISSET(listenfd, rset)){
		connfd = accept(listenfd, NULL, NULL);
		if(connfd < 0){
			cout << "accept error" << endl;
			exit(3);
		}

		/* wo should consider the biggest FD_SETSIZE
		if(i == FD_SETSIZE){
			cout << "too many clients" << endl;
			exit(3);
		}
		*/

		ClientInfo cli;
		cli.tcpfd = connfd;
		tcpfdcli.insert(make_pair(connfd, cli));

		FD_SET(connfd, allset);

		--nready;
		cout << "a new tcp accept" << endl;
	}

	return nready;
}

int tcp_event(map<int, ClientInfo> &tcpfdcli, map<int, ClientInfo> &udpfdcli, fd_set *rset, fd_set *allset, int nready)
{
	int sockfd;
	char buf[MAXLINE];

	uint8_t type;
	int64_t datalen;
	int size = sizeof(int64_t);

	for(auto it = tcpfdcli.begin(), end = tcpfdcli.end(); it != end; it++){
		sockfd = it->first;
		if(FD_ISSET(sockfd, rset)){
			if(Read(sockfd, &datalen, size, string("TCP")) != size){
				close(sockfd);
				FD_CLR(sockfd, allset);

				//clear tcp and udp if it exist, we need finish this later.
				//clear();
				cout << "some one left the chat !!!" << endl;
			}else{
				if(Read(sockfd, buf, datalen, string("TCP")) == datalen)
				{
					memcpy(&type, buf, 1);
					switch(type)
					{
						case LOGIN_TYPE:
							login(sockfd, tcpfdcli, udpfdcli, buf, datalen, allset);
							break;
						default:
							cout << "TCP unrecognised packet type" << endl;
							break;
					}
				}else{
					close(sockfd);
					FD_CLR(sockfd, allset);

					//clear tcp and udp if it exist, we need finish this later.
					//clear();
					
					cout << "some one left the chat !!!" << endl;
				}
			}

			if(--nready <= 0)
				break;
		}
	}
	return nready;
}

int udp_event(map<int, ClientInfo> &udpfdcli, fd_set *rset, fd_set *allset, int nready)
{
	int sockfd, n;
	char buf[MAXLINE];

	for(auto it = udpfdcli.begin(), end = udpfdcli.end(); it != end; it++){
		sockfd = it->first;
		if(FD_ISSET(sockfd, rset)){
			n = Read(sockfd, buf, MAXLINE-1, string("TCP"));
			if( n < 0 ){
				cout << "udp read error !!!" << endl;
			}else if(n == 0){
				close(sockfd);
				FD_CLR(sockfd, allset);

				//clear udp, we need finish this later.
				//clear();
				cout << "some one left the chat !!!" << endl;
			}else{
				cout << "udp receive msg" << endl;
				buf[n] = '\0';
				chatall(sockfd, udpfdcli, buf);
			}

			if(--nready <= 0)
				break;
		}
	}
	return nready;
}

int maxfd(map<int, ClientInfo> &tcpfdcli, map<int, ClientInfo> &udpfdcli, int listenfd)
{
	int max = -1;

	map<int, ClientInfo>::reverse_iterator it = tcpfdcli.rbegin();
	if(it != tcpfdcli.rend())
		max = it->first;

	it = udpfdcli.rbegin();
	if(it != udpfdcli.rend())
		max = it->first;

	return max > listenfd ? max : listenfd;
}

int main(int argc, char *argv[])
{
	map<int, ClientInfo> tcpfdcli;
	map<int, ClientInfo> udpfdcli;

	int listenfd, nready;
	listenfd = init_listen();

	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	
	while(1){
		rset = allset;
		nready = select(maxfd(tcpfdcli, udpfdcli, listenfd)+1, &rset, NULL, NULL, NULL);

		if( (nready = tcp_listen_event(listenfd, tcpfdcli, &rset, &allset, nready)) <= 0)
			continue;

		if( (nready = tcp_event(tcpfdcli, udpfdcli, &rset, &allset, nready)) <= 0)
			continue;

		if( (nready = udp_event(udpfdcli, &rset, &allset, nready)) <= 0)
			continue;

		cout << "WARNING: not all nready fds have been proccessed." << endl;
	}

}
