/*
Written 2009 by Brendan G Bohannon and hereby released into the public domain.
*/

#ifndef _NETINET_IN_H
#define _NETINET_IN_H

#include <sys/socket.h>

#define IPPROTO_IP 	0
#define IPPROTO_ICMP	1
#define IPPROTO_IGMP	2
#define IPPROTO_IPIP	4
#define IPPROTO_TCP	6
#define IPPROTO_EGP	8
#define IPPROTO_PUP	12
#define IPPROTO_UDP	17
#define IPPROTO_IDP	22
#define IPPROTO_RAW	255

#ifndef _IN_PORT_DEFINED
#define _IN_PORT_DEFINED
typedef uint16_t in_port_t;
#endif

#ifndef _IN_ADDR_DEFINED
#define _IN_ADDR_DEFINED
typedef uint32_t in_addr_t;
#endif

struct in_addr {
in_addr_t s_addr;
};

struct sockaddr_in {
sa_family_t sin_family;
in_port_t sin_port;
struct in_addr sin_addr;
char sin_zero[8];
};

struct in6_addr {
uint8_t s6_addr[16];
};

struct sockaddr_in6 {
sa_family_t sin6_family;
in_port_t sin6_port;
uint32_t sin6_flowinfo;
struct in6_addr sin6_addr;
uint32_t sin6_scope_id;
};


#endif


