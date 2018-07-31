#ifndef WRITE_H_
#define WRITE_H_
#include<string>
using std::string;

ssize_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, void *ptr, size_t nbytes, const string &type);

#endif
