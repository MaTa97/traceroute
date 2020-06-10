#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "error_wrapper.h"
#include "receive.h"


int wait_for_packets(int sockfd, struct timeval *timeout){
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);

    int ready = Select(sockfd + 1, &descriptors, NULL, NULL, timeout);

    return ready == 1;
}

struct icmp* get_icmp_header(struct ip* ip_header){
    return (struct icmp*)((u_int8_t*)ip_header + ip_header->ip_hl * 4);
}

int check_if_valid(u_int8_t *buffer, int ttl, int id, int* type){
    struct ip*    ip_header    = (struct ip*) buffer;
    struct icmp* icmp_header = get_icmp_header(ip_header);

    // if TTL = 0:
    // after 8 bits of icmp_type, 8 bits of icmp_code, 16 bits of icmp_checksum and 32 bits of unused
    // we have IP header + the first 64 bits of the original datagram's data.
    // so we have to move our pointer for 64 bits
    if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED){
        struct ip*   ip_header_next    = (struct ip*)((u_int8_t*)icmp_header + 8);
        struct icmp* icmp_header_next  = get_icmp_header(ip_header_next);
        if (icmp_header_next->icmp_id == id && icmp_header_next->icmp_seq == ttl){
            *type = ICMP_TIME_EXCEEDED;
            return EXIT_SUCCESS;
        }
    }
    else if(icmp_header->icmp_type == ICMP_ECHOREPLY 
            && icmp_header->icmp_id == id 
            && icmp_header->icmp_seq == ttl){
            *type = ICMP_ECHOREPLY;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int get_packets(int sockfd, int id, int ttl, struct  sockaddr_in *addr_replies, struct timeval *rtt, int* type){
    u_int8_t *buffer = malloc(IP_MAXPACKET * sizeof(u_int8_t));
    struct timeval begin, end, timeout;

    gettimeofday(&begin, NULL);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int n = 0;

    while (n < 3){
        if (!wait_for_packets(sockfd, &timeout))
            break;
        gettimeofday(&end, NULL);

        socklen_t reply_len = sizeof(addr_replies[n]);

        Recvfrom(
            sockfd, 
            buffer, 
            IP_MAXPACKET, 
            MSG_DONTWAIT, 
            (struct sockaddr*)&addr_replies[n], 
            &reply_len
        );

        if (check_if_valid(buffer, ttl, id, &type[n]) == EXIT_SUCCESS){
            timersub(&end, &begin, &rtt[n]);
            n++;
        }
    }
    free(buffer);
    return n;
}
