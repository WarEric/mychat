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

	return len;
}

bool decode_login_packet(LoginPacket &pkt, char buff[])
{
	char msg[MAXLINE];
	int len;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;

	//decode name
	memcpy(&len, ptr, 1);
	ptr += 1;
	if(len > MAXLINE)
		return false;

	memcpy(msg, ptr, len);
	ptr += len;
	pkt.name = string(msg);

	//decode passwd
	memcpy(&len, ptr, 1);
	ptr += 1;
	if(len > MAXLINE)
		return false;

	memcpy(msg, ptr, len);
	ptr += len;
	pkt.passwd = string(msg);

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
	int len;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;

	//decode result
	memcpy(&pkt.result, ptr, 1);
	ptr += 1;

	//decode msg
	memcpy(&len, ptr, 1);
	ptr += 1;
	if(len > MAXLINE)
		return false;

	memcpy(msg, ptr, len);
	ptr += len;
	pkt.msg = string(msg);

	return true;
}
