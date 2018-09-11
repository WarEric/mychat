/* by wrapping the original functions, we can make it easy used in our code.
 *
 * by wareric@163.com
 * 2018-09-10
 */
#ifndef WRAPIO_H_H
#define WRAPIO_H_H
#include <sys/types.h>
#include <sys/socket.h>

/*
 * my wrap sendto function, it will print error reason when error occurs.
 */
ssize_t sendto_wrap(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
#endif
