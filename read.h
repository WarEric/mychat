#ifndef READ_H_
#define READ_H_
#include<string>
using std::string;

ssize_t readn(int fd, void *vptr, size_t n);
ssize_t Read(int fd, void *ptr, size_t nbytes, const string &type);
#endif
