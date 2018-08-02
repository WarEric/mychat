#ifndef CLIENT_H_
#define CLIENT_H_
#include<string>
using std::string;

#define SERVPORT 5678
class ClientInfo{
	public:
		ClientInfo():servport(SERVPORT), udpfd(-1), tcpfd(-1){}
		ClientInfo(string nickname):name(nickname),servport(SERVPORT), udpfd(-1), tcpfd(-1){}

		string name;
		string passwd;
		string cliaddr;		//udp 
		string servaddr;	//udp and tcp
		int servport;		//udp and tcp
		int cliport;		//udp
		int udpfd;
		int tcpfd;
};
#endif
