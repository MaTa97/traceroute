#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "send.h"
#include "receive.h"
#include "error_wrapper.h"


int is_unique(char (*ip_strs)[20], int i, int n){
    for(int j = i + 1; j < n; j++)
        if(!strcmp(ip_strs[i], ip_strs[j]))
            return 0;
    return 1;
}

long count_avg(struct timeval* rtt){
    time_t tv_sec_sum = 0;
    suseconds_t tv_usec_sum = 0;
    for(int i = 0; i < 3; i++){
        tv_sec_sum  += rtt[i].tv_sec;
        tv_usec_sum += rtt[i].tv_usec;
    }
    return (tv_sec_sum * 1000 + tv_usec_sum / 1000) / 3;
}

void get_ips(struct sockaddr_in* addr, char (*ip_strs)[20], int n){
    for(int i = 0; i < n; i++)
        Inet_ntop(AF_INET, &(addr[i].sin_addr), ip_strs[i], sizeof(ip_strs[i]));
}

void print_reply(char (*ip_strs)[20], int ttl, long avg, int n){
    printf("%d. ", ttl);
    for(int i = 0; i < n; i++)
        if(is_unique(ip_strs, i, n))
            printf("%s ", ip_strs[i]);
    // no reply
    if (!n)
    printf("*\n");
    // some replies, not all
    else if (n < 3)
        printf("???\n");
    // all replies
    else
        printf("%ldms\n", avg);
}

int traceroute(int sockfd, struct sockaddr_in *addr){
    struct sockaddr_in replies[3];
    struct timeval rtt[3];
    int type[3];
    char ip_strs[3][20];
    pid_t id = Getpid();

    for(int i = 1; i < 31; i++){
        send_three_packets(sockfd, i, id, addr);
        int n = get_packets(sockfd, id, i, replies, rtt, type);
        get_ips(replies, ip_strs, n);
        long avg = 0;
        if (n == 3)
            avg = count_avg(rtt);
        print_reply(ip_strs, i, avg, n);

        if(n > 0 && type[0] == ICMP_ECHOREPLY)
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]){

    if (argc != 2){ print_usage(); }

    int sockfd = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_port = htons(7);
    addr.sin_family = AF_INET;

    Inet_pton(AF_INET, argv[1], &addr.sin_addr);

    exit(traceroute(sockfd, &addr));
}
