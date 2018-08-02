#ifndef CLIENTADDR_H_
#define CLIENTADDR_H_
#include<string>
#include<stdint.h>
using std::string;

class ClientAddr{
	public:
		friend bool operator < (const ClientAddr &addr1, const ClientAddr &addr2);

		ClientAddr(){}
		ClientAddr(const ClientAddr &);
		ClientAddr& operator=(const ClientAddr&);

		bool operator==(const ClientAddr &orig);
		bool operator!=(const ClientAddr &orig);
		bool operator>(const ClientAddr &orig);
		bool operator<(const ClientAddr &orig);

		string addr;
		uint16_t port;
};
#endif
