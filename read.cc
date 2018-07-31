#include<iostream>
#include<unistd.h>
#include"read.h"

using std::cout;
using std::endl;

ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = (char *)vptr;
	nleft = n;
	while(nleft > 0){
		if( (nread = read(fd, ptr, nleft)) < 0){
			if (errno == EINTR)
				nread = 0;
			else
				return (-1);
		}else if (nread == 0)
			break;

		nleft -= nread;
		ptr   += nread;
	}
	return (n - nleft);
}

ssize_t Read(int fd, void *ptr, size_t nbytes, const string &type)
{
	ssize_t		n;

	if( (n = readn(fd, ptr, nbytes)) < 0)
		cout << type << " read error" << endl;

	return (n);
}
