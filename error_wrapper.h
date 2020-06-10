#ifndef ERROR_WRAPPER_H
#define ERROR_WRAPPER_H

#include <sys/select.h> 
#include <sys/socket.h>

int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int Select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, 
            fd_set *restrict errorfds, struct timeval *restrict timeout); 
int Socket(int domain, int type, int protocol);
int Inet_pton(int af, const char *src, void *dst);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, 
               const struct sockaddr *dest_addr, socklen_t addrlen);    
ssize_t Recvfrom(int sockfd, void *restrict buffer, size_t len, int flags,
                struct sockaddr *restrict addr, socklen_t *restrict addr_len);
const char *Inet_ntop(int af, const void *src, char *dst, socklen_t size);
pid_t Getpid();
void print_usage();

#endif
