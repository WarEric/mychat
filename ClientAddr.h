/**
 * it record client address and port information. In this way,
 * we can quickly get it's client name by using it as a key of map.
 *
 * by wareric@163.com
 * 2018-09-12
 */
#ifndef CLIENTADDR_H_H
#define CLIENTADDR_H_H
#include <string>
#include <stdint.h>
using std::string;

class ClientAddr{
	public:
		ClientAddr(){}

		string addr;
		uint16_t port;
};
#endif
