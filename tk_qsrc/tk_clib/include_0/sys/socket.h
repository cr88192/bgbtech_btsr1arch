/*
Written 2009 by Brendan G Bohannon and hereby released into the public domain.
*/

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <stdint.h>
#include <sys/uio.h>
#include <sys/types.h>


#define SOCK_STREAM	1
#define SOCK_DGRAM	2
#define SOCK_RAW	3
#define SOCK_RDM	4
#define SOCK_SEQPACKET	5

#define AF_UNSPEC	0
#define AF_UNIX		1
#define AF_INET		2
#define AF_INET6	23

#define PF_UNSPEC	0
#define PF_UNIX		1
#define PF_INET		2
#define PF_INET6	23

#define SHUT_RD		0
#define SHUT_WR		1
#define SHUT_RDWR	2

#define MSG_OOB		0x0001
#define MSG_PEEK	0x0002
#define MSG_DONTROUTE	0x0004

#define MSG_TRUNC	0x0100
#define MSG_CTRUNC	0x0200

#define MSG_EOR		0x1000
#define MSG_WAITALL	0x2000

#define SOMAXCONN	0x7FFFFFFF

#define SOL_SOCKET	0xFFFF

#define SO_ACCEPTCONN	0x0002
#define SO_BROADCAST	0x0020
#define SO_DEBUG	0x0001
#define SO_DONTROUTE	0x0010
#define SO_ERROR	0x1007
#define SO_KEEPALIVE	0x0008
#define SO_LINGER	0x0080
#define SO_OOBINLINE	0x0100
#define SO_RCVBUF	0x1002
#define SO_RCVLOWAT	0x1004
#define SO_RCVTIMEO	0x1006
#define SO_REUSEADDR	0x0004
#define SO_SNDBUF	0x1001
#define SO_SNDLOWAT	0x1003
#define SO_SNDTIMEO	0x1005
#define SO_TYPE		0x1008

#define SO_USELOOPBACK	0x0040

#ifndef _SOCKLEN_DEFINED
#define _SOCKLEN_DEFINED
typedef int socklen_t;
#endif

#ifndef _SA_FAMILY_DEFINED
#define _SA_FAMILY_DEFINED
typedef uint16_t sa_family_t;
#endif

struct sockaddr {
sa_family_t sa_family;
char sa_data[14];
};

#define _SS_MAXSIZE 128
#define _SS_ALIGNSIZE (sizeof(int64_t))

#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof(sa_family_t))
#define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof(sa_family_t)+ \
                      _SS_PAD1SIZE + _SS_ALIGNSIZE))

struct sockaddr_storage {
sa_family_t  ss_family;  /* Address family. */
char _ss_pad1[_SS_PAD1SIZE];
int64_t _ss_align;
char _ss_pad2[_SS_PAD2SIZE];
};

struct msghdr {
void          *msg_name;
socklen_t      msg_namelen;
struct iovec  *msg_iov;
int            msg_iovlen;
void          *msg_control;
socklen_t      msg_controllen;
int            msg_flags;
};

struct cmsghdr {
socklen_t cmsg_len;
int cmsg_level;
int cmsg_type;
};


int     accept(int, struct sockaddr *, socklen_t *);
int     bind(int, const struct sockaddr *, socklen_t);
int     connect(int, const struct sockaddr *, socklen_t);
int     getpeername(int, struct sockaddr *, socklen_t *);
int     getsockname(int, struct sockaddr *, socklen_t *);
int     getsockopt(int, int, int, void *, socklen_t *);
int     listen(int, int);
ssize_t recv(int, void *, size_t, int);
ssize_t recvfrom(int, void *, size_t, int,
        struct sockaddr *, socklen_t *);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendmsg(int, const struct msghdr *, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
        socklen_t);
int     setsockopt(int, int, int, const void *, socklen_t);
int     shutdown(int, int);
int     socket(int, int, int);
int     sockatmark(int);
int     socketpair(int, int, int, int[2]);

#endif
