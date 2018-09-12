/**
 * 1. read infomation from config file in the format of map;
 * 2. line start with charater '#' will be regard as comment
 * line, datas are recorded in the format of "key = value ", it
 * doesn't matter whether it has blank space between words.
 *
 * by wareric@163.com
 * 2018-09-12
 * */
 
#ifndef SHADOW_H_H
#define SHADOW_H_H
#define COMMENT_CHAR '#'
#include <string>
#include <map>
 
using std::string;
using std::map;
 
bool readshadowfile(const string & filename, map<string, string> &m);
#endif
