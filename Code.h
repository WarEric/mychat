/*
 * abstract class to define the way of communication packets decoded.
 * by wareric@163.com
 * 2018-09-10
 */
#ifndef CODE_H_H
#define CODE_H_H
#include <stdint.h>
#include "Packet.h"

class Code{
	public:
		virtual int64_t encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_login_packet(LoginPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_auth_result_packet(AuthResultPacket &pkt, char buff[]) = 0;
};
#endif
