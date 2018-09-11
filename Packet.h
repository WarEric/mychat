/*
 * packet data structure
 * by wareric@163.com
 * 2018-09-10
 */
#ifndef PACKET_H_H
#define PACKET_H_H

#include <string>
#include <string.h>
#include <stdint.h>

using std::string;

#define TYPE_LOGIN				1
#define TYPE_AUTH_RESULT			2

#define AUTH_RESULT_PASSED                      0       // The user passed the verification.
#define AUTH_RESULT_ERROR_LOGGEDIN              1       // The user already has logged in.
#define AUTH_RESULT_ERROR_UNREGISTER            2       // This user has not registered
#define AUTH_RESULT_ERROR_WPASSWD               3       // Wrong passwd.

class LoginPacket{
	public:
		LoginPacket():type(TYPE_LOGIN){}

		uint8_t type;
		string name;
		string passwd;
		string addr;
		uint32_t msgport;		//used for establish connection and chat. udp.
		uint32_t heartport;		//udp
};

class AuthResultPacket{
	public:
		AuthResultPacket():type(TYPE_AUTH_RESULT){}

		uint8_t type;
		uint8_t result;
		uint32_t heartport;
		uint32_t chatport;
		string msg;
};
#endif