/*
 * by wareric@163.com
 * 2018-09-10
 */
#include <iostream>
#include <unistd.h>
#include "wrapio.h"

using std::cout;
using std::endl;

ssize_t sendto_wrap(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
	int res = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
	if(res != len)
	{
		cout << "udp sendto error res = " << res << endl;
		switch(errno){
			case EACCES:
				cout << "EACCES: Write permission is denied on the destination socket file, or search permission is denied for one of the directories the path prefix." << endl;
				break;

			//EAGAIN | EWOULDBLOCK
			case EAGAIN:
				cout << "EAGAIN: The socket referred to by sockfd had not previously been bound to an address and, upon attempting to bind it to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use." << endl;
				cout << "EWOULDBLOCK: The socket is marked nonblocking and the requested operation would block." << endl;
				break;

			case EALREADY:
				cout << "EALREADY: Another Fast Open is in progress." << endl;
				break;

			case EBADF:
				cout << "EBADF: sockfd is not a valid open file descriptor." << endl;
				break;

			case ECONNRESET:
				cout << "ECONNRESET: Connection reset by peer." << endl;
				break;

			case EDESTADDRREQ:
				cout << "EDESTADDRREQ: The socket is not connection-mode, and no peer address is set." << endl;
				break;

			case EFAULT:
				cout << "EFAULT: An invalid user space address was specified for an argument." << endl;
				break;

			case EINTR:
				cout << "EINTR: A signal occurred before any data was transmitted." << endl;
				break;

			case EINVAL:
				cout << "EINVAL: Invalid argument passed." << endl;
				break;

			case EISCONN:
				cout << "EISCONN: The connection-mode socket was connected already but a recipient was specifed." << endl;
				break;

			case EMSGSIZE:
				cout << "EMSGSIZE: The connection-mode socket was connected already but a recipient was specifed." << endl;
				break;

			case ENOBUFS:
				cout << "ENOBUFS: The output queue for a network interface was full. This generally indicates that the interface has stopped sending, but may be caused by transient congestion." << endl;
				break;

			case ENOMEM:
				cout << "ENOMEM: No memory available." << endl;
				break;

			case ENOTCONN:
				cout << "ENOTCONN: The socket is not connected, and no target has been given." << endl;
				break;

			case ENOTSOCK:
				cout << "ENOTSOCK: The file descriptor sockfd does not refer to a socket." << endl;
				break;

			case EOPNOTSUPP:
				cout << "EOPNOTSUPP: Some bit in the flags argument is inappropriate for the socket type." << endl;
				break;

			case EPIPE:
				cout << "EPIPE: The local end has been shut down on a connection oriented socket." << endl;
				break;

			default:
				cout << "unknown type" << endl;
				break;
		}
		return res;
	}

	return res;
}

ssize_t writen(int fd, const void *buf, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = (char *)buf;
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

ssize_t writen_wrap(int fd, void *ptr, size_t nbytes)
{
	if(writen(fd, ptr, nbytes) != nbytes)
	{
		cout << "writen_wrap error" << endl;
		return -1;
	}
}
