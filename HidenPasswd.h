/*
 * it doesn't print the input of passwd to screen. There is nothing shown when you typing except "ENTER" key,
 * Which imples we are finished.
 * by wareric@163.com
 * 2018-09-09
 */
#ifndef HIDENPASSWD_H_H
#define HIDENPASSWD_H_H

#include "Passwd.h"

class HidenPasswd : public Passwd{
	public:
		static HidenPasswd* getInstance();
		static void destoryInstance();

		bool getpasswd(string &passwd);

	private:
		HidenPasswd();
		static HidenPasswd* instance;
};

HidenPasswd* HidenPasswd::instance = nullptr;
#endif
