#include"ClientAddr.h"

bool ClientAddr::operator==(const ClientAddr &orig)
{
	if(this->addr != orig.addr)
		return false;

	if(this->port != orig.port)
		return false;

	return true;
}

bool ClientAddr::operator!=(const ClientAddr &orig)
{
	return !(*this == orig);
}
