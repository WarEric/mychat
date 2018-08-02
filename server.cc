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
#include<errno.h>
#include"server.h"
#include"packet.h"
#include"write.h"
#include"read.h"
#include"ClientInfo.h"
#include"ClientAddr.h"

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::make_pair;

bool recordAddr(ClientInfo &cli, map<ClientAddr, string> &addrcli)
{
	ClientAddr addr;
	addr.addr = cli.cliaddr;
	addr.port = cli.cliport;
	
	auto it = addrcli.find(addr);
	if(it != addrcli.end())
	{
		cout  << " already recorded addrinfo " << addr.addr << ":" << addr.port 
			<< " for " << it->second << endl;
		return false;
	}

	addrcli.insert(make_pair(addr, cli.name));
	return true;
}

bool getClientInfo(ClientInfo &cli, int sockfd, char buff[], int64_t datalen)
{
	LoginPacket pkt;
	if(decode_login_packet(pkt, buff) == false)
	{
		cout << "login packet decode error! " <<  endl;
		return false;
	}

	//get account
	cli.name = pkt.name;
	cli.passwd = pkt.passwd;
	cli.cliaddr = pkt.cliaddr;
	cli.cliport = pkt.cliport;
	cli.tcpfd = sockfd;

	return true;
}

bool login(int sockfd, map<int, ClientInfo> &tcpfdcli, map<ClientAddr, string> &addrcli, char buff[], int64_t datalen, fd_set *allset)
{
	ClientInfo cli;
	if(getClientInfo(cli, sockfd, buff, datalen) == false)
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
	//including name, passwd, ip format etc.
	//
	//
	//
	
	if(recordAddr(cli, addrcli) == false)
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

void showAndbroadcast(int udpfd, ClientAddr addr, map<ClientAddr, string> &addrcli, char msg[], int len)
{
	char buf[MAXLINE];
	auto it = addrcli.find(addr);
	if(it == addrcli.end())
	{
		cout << "an illegal ip address " << addr.addr << ":" << addr.port << " message :";
		Writen(fileno(stdout), msg, len, string("udp"));
		cout << "this message will be ignored." << endl;
		return ;
	}

	msg[len] = '\0';
	int n = snprintf(buf, MAXLINE, "%s:%s", it->second.c_str(), msg);
	if(n > MAXLINE)
	{
		cout << it->second << "(" << addr.addr << ":" << addr.port << ")'s message is long than buffer, we will ignore it" << endl;
		return ;
	}


	struct sockaddr_in cliaddr;
	int clilen = sizeof(cliaddr);
	int res;
	for(auto iter = addrcli.begin(); iter != addrcli.end(); ++iter)
	{
		if(iter == it)
		{
			Writen(fileno(stdout), buf, n, string("show"));
		}else{
			bzero(&cliaddr, clilen);
			cliaddr.sin_family = AF_INET;
			cliaddr.sin_port = htons((iter->first).port);
			inet_pton(AF_INET, (iter->first).addr.c_str(), &cliaddr.sin_addr);
			res = sendto(udpfd, buf, n, 0, (struct sockaddr *)&cliaddr, clilen);
			if(res != n)
			{
				cout << "udp sendto error res = " << res << endl;
				switch(errno){
					case EBADF:
						cout << "EBADF" << endl;
						break;
					case EFAULT:
						cout << "EFAULT" << endl;
						break;
				//	case WNOTSOCK:
				//		cout << "WNOSTOCK" << endl;
				//		break;
					case EINTR:
						cout << "EINTR" << endl;
						break;
					case EAGAIN:
						cout << "EAGAIN" << endl;
						break;
					case ENOBUFS:
						cout << "ENOBUFS" << endl;
						break;
					case EINVAL:
						cout << "EINVAL" << endl;
						break;
					default:
						break;
				}
			}
		}
	}
	
}

int init_tcp_listen()
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

int init_udp_bind()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		cout << "udp socket error" << endl;
		exit(3);
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "udp bind error" << endl;
		exit(4);
	}
	return sockfd;
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
	}

	return nready;
}

int tcp_event(map<int, ClientInfo> &tcpfdcli, map<ClientAddr, string> &addrcli, fd_set *rset, fd_set *allset, int nready)
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
							login(sockfd, tcpfdcli, addrcli, buf, datalen, allset);
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

int udp_event(int sockfd, map<ClientAddr, string> &addrcli, fd_set rset, int nready)
{
	int n;
	socklen_t len;
	char buf[MAXLINE];

	if(!FD_ISSET(sockfd, &rset))
		return nready;

	for(; nready > 0 ; nready--)
	{
		struct sockaddr_in cliaddr;
		len = sizeof(cliaddr);
		if( (n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) < 0)
		{
			cout << "udp receive error" << endl;
			continue;
		}else if(n == 0)
		{
			break;
		}
		
		ClientAddr addr;
		addr.addr = string(inet_ntoa(cliaddr.sin_addr));
		addr.port = ntohs(cliaddr.sin_port);
		showAndbroadcast(sockfd, addr, addrcli, buf, n);
	}

	return nready;
}

int maxfd(int tcplistenfd,int udplistenfd, map<int, ClientInfo> &tcpfdcli)
{
	int max = tcplistenfd > udplistenfd ? tcplistenfd : udplistenfd;

	map<int, ClientInfo>::reverse_iterator it = tcpfdcli.rbegin();
	if(it != tcpfdcli.rend())
	{
		if(it->first > max)
		max = it->first;
	}

	return max;
}

int main(int argc, char *argv[])
{
	map<int, ClientInfo> tcpfdcli;
	map<ClientAddr, string> addrcli;//addr to account name

	int tcplistenfd, udplistenfd, nready;
	tcplistenfd = init_tcp_listen();
	udplistenfd = init_udp_bind();

	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(tcplistenfd, &allset);
	FD_SET(udplistenfd, &allset);
	
	while(1){
		rset = allset;
		nready = select(maxfd(tcplistenfd, udplistenfd, tcpfdcli)+1, &rset, NULL, NULL, NULL);

		if( (nready = tcp_listen_event(tcplistenfd, tcpfdcli, &rset, &allset, nready)) <= 0)
			continue;

		if( (nready = tcp_event(tcpfdcli, addrcli, &rset, &allset, nready)) <= 0)
			continue;

		if( (nready = udp_event(udplistenfd, addrcli, rset, nready)) <= 0)
			continue;

		cout << "WARNING: not all nready fds have been proccessed." << endl;
	}

}
