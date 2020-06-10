#ifndef SEND_H
#define SEND_H

u_int16_t compute_icmp_checksum (const void *buff, int length);
void send_packet(int sockfd, int ttl, int id, struct sockaddr_in* addr);
void send_three_packets(int sockfd, int ttl, int id, struct sockaddr_in* addr);

#endif
