/*
 * A CLI client program. With the help of udp protocol. you can use this client to chat with others.
 * by wareric@163.com
 * 2018-09-06
 */
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "client.h"
#include "wrapio.h"
#include "ByteOrderCode.h"
#include "HidenPasswd.h"
#include "Packet.h"

using std::cout;
using std::cin;
using std::endl;

int confd = -1;	//control fd
int chatfd = -1;
int heartfd = -1;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "input format is wrong, example like this:\n"
			<< "./client <IPaddress>" << endl;
		exit(EXIT_CLIENT_INPUT_ARG_WRONG);
	}
	char *servaddr = argv[1];

	signal(SIGINT, signalHandler);
	
	Code* code = ByteOrderCode::getInstance();
	Passwd* passwd = HidenPasswd::getInstance();

	if(login(code, passwd, servaddr, SERVER_LOGIN_PORT) == false)
	{
		cout << "login failure, we will exit now." << endl;
		exit(EXIT_CLIENT_LOGIN_FAILURE);
	}

	return 0;
}

bool login(Code* code, Passwd* passwd, char *servaddr, int servport)
{
	if((confd = udpsocket()) < 0)
	{
		cout << "get confd socket error." << endl;
		return false;
	}

	if(udpconnect(confd, string(servaddr), servport) == false)
	{
		cout << "confd connect error." << endl;
		return false;
	}

	if((chatfd = udpsocket()) < 0)
	{
		cout << "get chatfd socket error." << endl;
		return false;
	}

	if((heartfd = udpsocket()) < 0)
	{
		cout << "get heartfd socket error." << endl;
		return false;
	}

	LoginPacket logpkt;
	if( (logpkt.chatport = getsockport(chatfd)) < 0)
	{
		cout << "get chatfd port wrong" << endl;
		return false;
	}

	if( (logpkt.heartport = getsockport(heartfd)) < 0)
	{
		cout << "get hearfd port wrong" << endl;
		return false;
	}

	AuthResultPacket res;
	for(int iter = 0;;)
	{
		do{
			logpkt.name.clear();
			logpkt.passwd.clear();

			cout << "login:";
			cin >> logpkt.name;
		}while(passwd->getpasswd(logpkt.passwd) != true);

		char buff[MAXLINE];
		int datalen = code->encode_login_packet(logpkt, buff, sizeof(buff));
		if(datalen < 0)
		{
			cout << "encode login packet wrong" << endl;
			return false;
		}

		//send login msg to server
		if(writen_wrap(confd, buff, datalen) < 0)
		{
			cout << "write confd error" << endl;
			return false;
		}
		
		//get feedback from server
		if(read(confd, buff, MAXLINE) == 0){
			cout << "server terminated" << endl;
			//throw a signal.
			return false;
		}

		if(code->decode_auth_result_packet(res, buff) == false)
		{
			cout << "auth result decode error!" << endl;
			return false;
		}
		
		if(res.type != TYPE_AUTH_RESULT)
		{
			cout << "unmatch result type of login" << endl;
			return false;
		}
		
		if(res.result == AUTH_RESULT_PASSED)
			break;
		
		//we could do more here.
		cout << "login failure." << endl;
		cout << res.msg << endl;	
		
		if(++iter >= LOGIN_TRY_MAX_NUMBER)
			return false;
	}
	
	cout << "login successfully." << endl;
	cout << res.msg << endl;

	if(success_login_connect(chatfd, res.chatport, heartfd, res.heartport, string(servaddr)) == false)
	{
		cout << "connect chatfd and heartfd error." << endl;
		//we should notify server we will exit.
		return false;
	}
	return true;
}

int udpsocket()
{
	return socket(AF_INET, SOCK_DGRAM, 0);
}

/*
 * string addr is a presentation ip like 192.168.1.1, not bin code.
 */
bool udpconnect(int fd, string addr, int port)
{
	int n = 0;

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	if((n = inet_pton(AF_INET, addr.c_str(), &servaddr.sin_addr)) < 0)
	{
		cout << "convert presentation " << addr << " to numeric wrong." << endl;
		return false;
	}else if(n == 0){
		cout << "invalid ip address: " << addr;
		return false;
	}
	if(connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout << "udp connect error" << endl;
		return false;
	}
	return true;
}

/*
 * connect heartfd and chatfd
 */
bool success_login_connect(int chatfd, int chatport, int heartfd, int heartport, string addr)
{
	if(udpconnect(chatfd, addr, chatport) == false)
	{
		cout << "chatfd connect error" << endl;
		return false;
	}

	if(udpconnect(heartfd, addr, heartport) == false)
	{
		cout << "heartfd connect error" << endl;
		return false;
	}

	return true;
}

int getsockport(int fd)
{
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	if(getsockname(fd, (struct sockaddr*)&cliaddr, &len) == -1)
	{
		cout << "udpfd(" << fd << ") can't getsockname." << endl;
		return -1;
	}

	return ntohs(cliaddr.sin_port);
}

string getsockaddr(int fd)
{
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	if(getsockname(fd, (struct sockaddr*)&cliaddr, &len) == -1)
	{
		cout << "udpfd(" << fd << ") can't getsockname." << endl;
		return string();
	}
	return string(inet_ntoa(cliaddr.sin_addr));
}

void clearfd_exit(int num)
{
	cout << "the program is exiting..." << endl;
	if(confd >= 0)
	{
		//notify server
		close(confd);
	}

	if(heartfd >= 0)
		close(heartfd);

	if(chatfd >= 0)
		close(chatfd);

}

void signalHandler(int signum)
{
	cout << "\nwe will exit from this chat" << endl;
	clearfd_exit(0);
}
