/*
 * abstract class to define the way of communication packets decoded.
 * by wareric@163.com
 * 2018-09-10
 */
#ifndef CODE_H_H
#define CODE_H_H
#include <stdint.h>
#include "Packet.h"
#include "ServerPacket.h"

class Code{
	public:
		virtual uint8_t getType(char buff[]) = 0;

		//inet socket domain packet
		virtual int64_t encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_login_packet(LoginPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_logout_packet(const LogoutPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_logout_packet(LogoutPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_auth_result_packet(AuthResultPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_shutdown_packet(const ShutdownPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_shutdown_packet(ShutdownPacket &pkt, char buff[]) = 0;


		//server unix socket domain packet
		virtual int64_t encode_startup_packet(const StartUpPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_startup_packet(StartUpPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_exist_packet(const ExistPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_exist_packet(ExistPacket &pkt, char buff[]) = 0;

		virtual int64_t encode_broadcast_packet(const BroadcastPacket &pkt, char buff[], size_t maxlen) = 0;
		virtual bool decode_broadcast_packet(BroadcastPacket &pkt, char buff[]) = 0;
};
#endif
