#ifndef __TK_NETINET_IN_H
#define __TK_NETINET_IN_H

#include <sys/socket.h>

typedef	uint16_t	in_port_t;
typedef	uint32_t	in_addr_t;

struct in_addr {
in_addr_t  s_addr;
};

struct in6_addr {
uint8_t s6_addr[16];
};

struct sockaddr_in {
sa_family_t		sin_family;
in_port_t		sin_port;
struct in_addr	sin_addr;
};

struct sockaddr_in6 {
sa_family_t			sin6_family;
in_port_t			sin6_port;
uint32_t			sin6_flowinfo;
struct in6_addr	sin6_addr;
uint32_t			sin6_scope_id;
};

extern struct in6_addr in6addr_any;
extern struct in6_addr in6addr_loopback;

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

in_addr_t	inet_addr(const char *);
char		*inet_ntoa(struct in_addr);
const char	*inet_ntop(int, const void *restrict, char *restrict, socklen_t);
int			inet_pton(int, const char *restrict, void *restrict);

#endif
