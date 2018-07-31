#ifndef PACKET_H_
#define PACKET_H_
#include<string>
#include<stdint.h>

#define LOGIN_TYPE 1
#define AUTH_RESULT_TYPE 2

using std::string;

class LoginPacket{
	public:
		LoginPacket(const string &account, const string &passkey):
       			type(LOGIN_TYPE), name(account), passwd(passkey){}
		LoginPacket():type(LOGIN_TYPE){}

		uint8_t type;
		string name;
		string passwd;
};

int64_t encode_login_packet(const LoginPacket &pkt, char buff[], size_t maxlen);
bool decode_login_packet(LoginPacket &pkt, char buff[]);

class AuthResultPacket{
	public:
		AuthResultPacket():type(AUTH_RESULT_TYPE){}

		uint8_t type;
		uint8_t result;
		string msg;
};

int64_t encode_auth_result_packet(const AuthResultPacket &pkt, char buff[], size_t maxlen);
bool decode_auth_result_packet(AuthResultPacket &pkt, char buff[]);

#endif
