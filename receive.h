#ifndef  RECEIVE_H
#define RECEIVE_H

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

int wait_for_packets(int sockfd, struct timeval *timeout);
struct icmp* get_icmp_header(struct ip* ip_header);
int check_if_valid(u_int8_t *buffer, int ttl, int id, int* type);
int get_packets(int sockfd, int id, int ttl, struct  sockaddr_in *addr_replies, struct timeval *rtt, int* type);

#endif
