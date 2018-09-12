/**
 * Packet for communication between server_* by unix domain socket.
 *
 * by wareric@163.com
 * 2018-9-12
 */
#ifndef SERVERPACKET_H_H
#define SERVERPACKET_H_H

#include <stdint.h>

#define TYPE_SERVER_STARTUP	100	// server_* startup info
#define TYPE_SERVER_EXIST	101	// clients' join or leave
#define TYPE_SERVER_BROADCAST	102	// broadcast this msg to all online clients


class StartUpPacket{
	public:
		StartUpPacket(uint32_t port):type(TYPE_SERVER_STARTUP), inport(port){}

		uint8_t type;
		uint32_t inport;		//inet port
};

/**
 * client's join and leave info.
 * @exist 1 represent join, 0 represent leave.
 */
class ExistPacket{
	public:
		ExistPacket():type(TYPE_SERVER_EXIST){}

		uint8_t type;
		uint8_t exist;
		string name;
		string addr;
		uint32_t port;			//heartport for server_heart or chatport for server_chat
};

class BroadcastPacket{
	public:
		BroadcastPacket():type(TYPE_SERVER_BROADCAST){}
		BroadcastPacket(string message):type(TYPE_SERVER_BROADCAST), msg(message){}

		uint8_t type;
		string msg;
};
#endif
