#include"ClientAddr.h"

ClientAddr::ClientAddr(const ClientAddr &orig):addr(orig.addr), port(orig.port)
{
}

ClientAddr& ClientAddr::operator=(const ClientAddr &orig)
{
	addr = orig.addr;
	port = orig.port;

	return *this;
}

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

bool ClientAddr::operator>(const ClientAddr &orig)
{
	if(addr > orig.addr)
	{
		return true;
	}else if(addr < orig.addr){
		return false;
	}else{
		if(port > orig.port)
			return true;
		else
			return false;
	}
}

bool ClientAddr::operator<(const ClientAddr &orig)
{
	return !(*this > orig);
}

bool operator < (const ClientAddr &addr1, const ClientAddr &addr2)
{
	if(addr1.addr < addr2.addr)
	{
		return true;
	}else if(addr1.addr > addr2.addr){
		return false;
	}else{
		if(addr1.port < addr2.port)
			return true;
		else
			return false;
	}
}
