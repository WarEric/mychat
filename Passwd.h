/*
 * get passwd from cli
 * by wareric@163.com
 * 2018-09-09
 */
#ifndef PASSWD_H_H
#define PASSWD_H_H

#include <iostream>
using std::string;

class Passwd{
	public:
		virtual bool getpasswd(string &passwd) = 0;
};
#endif
