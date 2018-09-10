/*
 * It deeply relys on the byte order of a basic data type.
 * By default, most of computers use little endian pattern
 * and this code works well. Unfortunatelly, there must be
 * some big endian machines, which will explain the byte
 * order wrong and get a wrong data.
 *
 * This code is just a temporary way. We will offer better
 * solutions.
 *
 * by wareric@163.com
 * 2018-09-10
 */
#ifndef BYTEORDERCODE_H_H
#define BYTEORDERCODE_H_H
#include "Code.h"
class ByteOrderCode: public Code{
	public:
		static ByteOrderCode* getInstance();
		static void destoryInstance();

		int64_t encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen);
		bool decode_login_packet(LoginPacket &pkt, char buff[]);
		
		int64_t encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen);
		bool decode_auth_result_packet(AuthResultPacket &pkt, char buff[]);

	private:
		ByteOrderCode();
		static ByteOrderCode* instance;
};

ByteOrderCode* ByteOrderCode::instance = nullptr;
#endif
