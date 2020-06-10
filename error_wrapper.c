#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error_wrapper.h"

int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    int res = setsockopt(sockfd, level, optname, optval, optlen);
    if (res < 0){
        fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

int Select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, 
            fd_set *restrict errorfds, struct timeval *restrict timeout)
{
    int res = select(nfds, readfds, writefds, errorfds, timeout);
    if (res < 0){
        fprintf(stderr, "select error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

int Inet_pton(int af, const char *src, void *dst){
    int res = inet_pton(af, src, dst);
    if (res == 0){
        printf("Wrong IP: %s\n", src);
        print_usage();
    }
    else if (res < 0){
        fprintf(stderr, "inet_pton error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

int Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if (res < 0){
        fprintf(stderr, "socket error: %s\n", strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, 
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    ssize_t res = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (res < 0){
        fprintf(stderr, "sendto error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Recvfrom(int sockfd, void *restrict buffer, size_t len, int flags,
                struct sockaddr *restrict addr, socklen_t *restrict addr_len)
{
    ssize_t res = recvfrom(sockfd, buffer, len, flags, addr, addr_len);
    if(res < 0 && (errno != EAGAIN || errno != EWOULDBLOCK)){
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

const char *Inet_ntop(int af, const void *src, char *dst, socklen_t size){
    const char* res = inet_ntop(af, src, dst, size);
    if(res == NULL){
        fprintf(stderr, "inet_ntop error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

pid_t Getpid(){
    pid_t pid = getpid();
    if (pid < 0){
        fprintf(stderr, "getpid error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return pid;
}

void print_usage(){
    fprintf(stderr, "Usage: ./traceroute x.x.x.x \n");
    exit(EXIT_FAILURE);
}
