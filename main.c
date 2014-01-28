#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void usage(const char *app)
{
    printf("Usage: %s <IP>/<Bits>\n", app);
    exit(1);
}

int main(int argc, char const *argv[])
{
    struct in_addr ip, net, subnetmask, broadcast, first, last;
    int bits;
    int ret;
    char *szIP;
    int a, b, c, d;
    int i;

    if (argc < 2)
        usage(argv[0]);

    ret = sscanf(argv[1], "%i.%i.%i.%i/%i", &a, &b, &c, &d, &bits);
    if (ret != 5 ) {
        fprintf(stderr, "Error: Invalid syntax.\n");
        usage(argv[0]);
    }

    ip.s_addr = a << 24 | b << 16 | c << 8 | d;
    subnetmask.s_addr = 0xffffffff;
    subnetmask.s_addr <<= (32-bits);
    net = ip;
    net.s_addr &= subnetmask.s_addr;
    broadcast = ip;
    broadcast.s_addr |= (~subnetmask.s_addr);
    first = net;
    first.s_addr += 1;
    last = broadcast;
    last.s_addr -=1;

    ip.s_addr         = htonl(ip.s_addr);
    subnetmask.s_addr = htonl(subnetmask.s_addr);
    net.s_addr        = htonl(net.s_addr);
    broadcast.s_addr  = htonl(broadcast.s_addr);
    first.s_addr      = htonl(first.s_addr);
    last.s_addr       = htonl(last.s_addr);

    szIP = inet_ntoa(ip);
    printf("IP: %s\n", szIP);
    szIP = inet_ntoa(subnetmask);
    printf("Subnetmask: %s\n", szIP);
    szIP = inet_ntoa(net);
    printf("Network address: %s\n", szIP);
    szIP = inet_ntoa(broadcast);
    printf("Broadcast address: %s\n", szIP);
    szIP = inet_ntoa(first);
    printf("Usable range: %s - ", szIP);
    szIP = inet_ntoa(last);
    printf("%s\n", szIP);
    printf("(Note: first address is typically the gateway/router)\n");

    return 0;
}

