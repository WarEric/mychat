#ifndef PACKET_H_
#define PACKET_H_
#include<string>
#include<stdint.h>

#define LOGIN_TYPE 1
#define AUTH_RESULT_TYPE 2

#define AUTH_RESULT_PASSED			0	// The user passed the verification.
#define AUTH_RESULT_ERROR_LOGGEDIN		1	// The user already has logged in.
#define AUTH_RESULT_ERROR_UNREGISTER		2	// This user has not registered
#define AUTH_RESULT_ERROR_WPASSWD		3	// Wrong passwd.
#define AUTH_RESULT_ERROR_UDPCON		4	// Server can't establish udp connection.
#define AUTH_RESULT_ERROR_GETINFO		5	// Server can't get clientinfo.

using std::string;

class LoginPacket{
	public:
		LoginPacket(const string &account, const string &passkey, const string &addr, uint32_t port):
       			type(LOGIN_TYPE), name(account), passwd(passkey), cliaddr(addr), cliport(port){}
		LoginPacket():type(LOGIN_TYPE){}

		uint8_t type;
		string name;
		string passwd;
		string cliaddr;
		uint32_t cliport;
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
