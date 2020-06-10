#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <assert.h>

#include "send.h"
#include "error_wrapper.h"

u_int16_t compute_icmp_checksum (const void *buff, int length){
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void send_packet(int sockfd, int ttl, int id, struct sockaddr_in* addr){
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_id = id;
    header.icmp_seq = ttl;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(&header));

    Setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    Sendto(
        sockfd,
        &header,
        sizeof(&header),
        0,
        (struct sockaddr*)addr,
        sizeof(*addr)
    );
}

void send_three_packets(int sockfd, int ttl, int id, struct sockaddr_in* addr){
    for(int i = 0; i < 3; i++)
        send_packet(sockfd, ttl, id, addr);
}
