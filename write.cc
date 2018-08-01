#include<unistd.h>
#include<iostream>
#include<errno.h>
#include"write.h"
using std::cout;
using std::endl;

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = (char *)vptr;
	nleft = n;
	while(nleft > 0){
		if( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return (-1);
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return (n);
}

void Writen(int fd, void *ptr, size_t nbytes, const string &type)
{
	if(writen(fd, ptr, nbytes) != nbytes)
	{
		cout << type << " writen error" << endl;
	}
}
