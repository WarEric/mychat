/**
 * This a server functions library
 * by wareric@163.com
 * 2018-09-05
 * */
#ifndef SERVER_H_H
#define SERVER_H_H

#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

using std::cout;
using std::endl;
using std::string;

/*
 * return < 0, error
 * return >= 0, success
 *  */
int init_unix_bind(char *path);
#endif
