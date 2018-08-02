#ifndef CLIENTADDR_H_
#define CLIENTADDR_H_
#include<string>
#include<stdint.h>
using std::string;

class ClientAddr{
	public:
		bool operator==(const ClientAddr &orig);
		bool operator!=(const ClientAddr &orig);

		string addr;
		uint32_t port;
};
#endif
