/*
 * by wareric@163.com
 * 2018-09-10
 */

#include "ByteOrderCode.h"
#define MAXLINE 4096

ByteOrderCode* ByteOrderCode::instance = nullptr;

ByteOrderCode* ByteOrderCode::getInstance()
{
	if(instance == nullptr)
		instance = new ByteOrderCode();
	return instance;
}

void ByteOrderCode::destoryInstance()
{
	if(instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

uint8_t ByteOrderCode::getType(char buff[])
{
	int8_t type;
	memcpy(&type, buff, sizeof(uint8_t));
	return type;
}

int64_t ByteOrderCode::encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen)
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

	//encode chatport
	memcpy(ptr, &pkt.chatport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	//encode heartport
	memcpy(ptr, &pkt.heartport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	return len;
}

bool ByteOrderCode::decode_login_packet(LoginPacket &pkt, char buff[])
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

	//decode chatport
	memcpy(&pkt.chatport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

	//decode heartport
	memcpy(&pkt.heartport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

	return true;
}

int64_t ByteOrderCode::encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen)
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

	//encode heartport
	memcpy(ptr, &pkt.heartport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	//encode chatport
	memcpy(ptr, &pkt.chatport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
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

bool ByteOrderCode::decode_auth_result_packet(AuthResultPacket &pkt, char buff[])
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

	//decode heartport
	memcpy(&pkt.heartport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

	//decode chatport
	memcpy(&pkt.chatport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

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

int64_t ByteOrderCode::encode_startup_packet(const StartUpPacket &pkt, char buff[], size_t maxlen)
{	
	int64_t len = 0;
	char *ptr = buff;

	//encode type
	memcpy(ptr, &pkt.type, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	//encode inport
	memcpy(ptr, &pkt.inport, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	return len;
}

bool ByteOrderCode::decode_startup_packet(StartUpPacket &pkt, char buff[])
{
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;

	//decode inport
	memcpy(&pkt.inport, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);

	return true;
}

int64_t ByteOrderCode::encode_exist_packet(const ExistPacket &pkt, char buff[], size_t maxlen)
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

	//encode exist
	memcpy(ptr, &pkt.exist, sizeof(uint8_t));
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

	//encode addr
	strlen = pkt.addr.size()+1;// add \0
	memcpy(ptr, &strlen, 1);
	ptr += 1;
	len += 1;
	if(len > maxlen)
		return -1;

	memcpy(ptr, pkt.addr.c_str(), strlen);
	ptr += strlen;
	len += strlen;
	if(len > maxlen)
		return -1;

	//encode port
	memcpy(ptr, &pkt.port, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	len += sizeof(uint32_t);
	if(len > maxlen)
		return -1;

	return len;
}

bool ByteOrderCode::decode_exist_packet(ExistPacket &pkt, char buff[])
{
	char msg[MAXLINE];
	uint8_t strlen;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
	ptr += 1;


	//decode exist
	memcpy(&pkt.exist, ptr, sizeof(uint8_t));
	ptr += sizeof(uint8_t);

	//decode name
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.name = string(msg);

	//decode addr
	memcpy(&strlen, ptr, 1);
	ptr += 1;
	if(strlen > MAXLINE)
		return false;

	memcpy(msg, ptr, strlen);
	ptr += strlen;
	pkt.addr = string(msg);

	//decode port
	memcpy(&pkt.port, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	return true;
}

int64_t ByteOrderCode::encode_broadcast_packet(const BroadcastPacket &pkt, char buff[], size_t maxlen)
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

bool ByteOrderCode::decode_broadcast_packet(BroadcastPacket &pkt, char buff[])
{
	char msg[MAXLINE];
	uint8_t strlen;
	char *ptr = buff;

	//decode type
	memcpy(&pkt.type, ptr, 1);
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
