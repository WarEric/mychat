/*
 * hiden passwd
 * by wareric@163.com
 * 2018-09-09
 */
#include <unistd.h>
#include "HidenPasswd.h"

bool HidenPasswd::getpasswd(string &passwd)
{
	char *pw = getpass("passwd:");
	passwd = string(pw);

	if(passwd.empty())
		return false;
	else
		return true;
}
