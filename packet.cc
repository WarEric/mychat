#include<iostream>
#include<string.h>
#include<string>
#include"packet.h"
#define MAXLINE 4096

using std::string;

int64_t encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen)
{
	uint8_t strlen = 0;
	int64_t len = 0;
	char *ptr = buff;

	//encode type
	memcpy(ptr, &pkt.type, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	//encode name
	strlen = pkt.name.size()+1;// add \0
	memcpy(ptr, &strlen, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	memcpy(ptr, pkt.name.c_str(), strlen);
	ptr += strlen;
	len += strlen;
	if(len > maxlen)
		return -1;

	//encode passwd
	strlen = pkt.passwd.size()+1;// add \0
	memcpy(ptr, &strlen, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	memcpy(ptr, pkt.passwd.c_str(), strlen);
	ptr += strlen;
	len += strlen;
	if(len > maxlen)
		return -1;


	//encode cliaddr;
	strlen = pkt.cliaddr.size()+1;// add \0
	memcpy(ptr, &strlen, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	memcpy(ptr, pkt.cliaddr.c_str(), strlen);
	ptr += strlen;
	len += strlen;
	if(len > maxlen)
		return -1;

	//encode cliport
	memcpy(ptr, &pkt.cliport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	return len;
}

bool decode_login_packet(LoginPacket &pkt, char buff[])
{
	char msg[MAXLINE];
	uint8_t strlen;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;

	//decode name
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.name = string(msg);

	//decode passwd
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.passwd = string(msg);

	//decode cliaddr
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.cliaddr = string(msg);

	//decode cliport
	memcpy(&pkt.cliport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

	return true;
}

int64_t encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen)
{	
	uint8_t strlen = 0;
	int64_t len = 0;
	char *ptr = buff;

	//encode type
	memcpy(ptr, &pkt.type, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	//encode result
	memcpy(ptr, &pkt.result, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;


	//encode msg
	strlen = pkt.msg.size()+1;// add \0
	memcpy(ptr, &strlen, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	memcpy(ptr, pkt.msg.c_str(), strlen);
	ptr += strlen;
	len += strlen;
	if(len > maxlen)
		return -1;

	return len;
}

bool decode_auth_result_packet(AuthResultPacket &pkt, char buff[])
{
	char msg[MAXLINE];
	uint8_t strlen;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;

	//decode result
	memcpy(&pkt.result, ptr, 1);
	ptr += 1;

	//decode msg
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.msg = string(msg);

	return true;
}
