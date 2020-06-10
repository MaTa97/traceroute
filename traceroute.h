#ifndef TRACEROUTE_H
#define TRACEROUTE_H

int is_unique(char (*ip_strs)[20], int i, int n);
long count_avg(struct timeval* rtt);
void get_ips(struct sockaddr_in* addr, char (*ip_strs)[20], int n);
void print_reply(char (*ip_strs)[20], int ttl, long long avg, int n);
int traceroute(int sockfd, struct sockaddr_in* addr);

#endif
