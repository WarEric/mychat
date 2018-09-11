/*
 * hiden passwd
 * by wareric@163.com
 * 2018-09-09
 */
#include <unistd.h>
#include "HidenPasswd.h"

HidenPasswd* HidenPasswd::instance = nullptr;

HidenPasswd* HidenPasswd::getInstance()
{
	if(instance == nullptr)
		instance = new HidenPasswd();
	return instance;
}

void HidenPasswd::destoryInstance()
{
	if(instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

bool HidenPasswd::getpasswd(string &passwd)
{
	char *pw = getpass("passwd:");
	passwd = string(pw);

	if(passwd.empty())
		return false;
	else
		return true;
}
