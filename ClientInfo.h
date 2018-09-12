/*
 * client info including name, passwd, ip, udpport
 * by wareric@163.com
 * 2018-09-06
 */
#ifndef CLIENT_H_
#define CLIENT_H_
#include <string>
#include <stdint.h>
using std::string;

class ClientInfo{
	public:
		string name;
		string passwd;
		string addr;
		uint32_t connport;
		uint32_t chatport;		//udp
		uint32_t heartport;		//udp
};
#endif
