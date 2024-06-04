#ifndef __TK_SYS_SOCKET_H
#define __TK_SYS_SOCKET_H

#include <sys/types.h>


#define SOCK_STREAM		1
#define SOCK_DGRAM		2
#define SOCK_RAW		3
#define SOCK_SEQPACKET	5

#define SOL_SOCKET		1

#define SOMAXCONN		255

#define AF_INET			2
#define AF_INET6		10
#define AF_UNIX			1
#define AF_UNSPEC		0

#define AF_LOCAL		AF_UNIX


#if 0
SO_ACCEPTCONN
SO_BROADCAST
SO_DEBUG
SO_DONTROUTE
SO_ERROR
SO_KEEPALIVE
SO_LINGER
SO_OOBINLINE
SO_RCVBUF
SO_RCVLOWAT
SO_RCVTIMEO
SO_REUSEADDR
SO_SNDBUF
SO_SNDLOWAT
SO_SNDTIMEO
SO_TYPE

MSG_CTRUNC
MSG_DONTROUTE
MSG_EOR
MSG_OOB
MSG_PEEK
MSG_TRUNC
MSG_WAITALL

SHUT_RD
SHUT_RDWR
SHUT_WR
#endif

typedef short		sa_family_t;
// typedef size_t		socklen_t;
typedef int		socklen_t;

struct sockaddr_storage {
	sa_family_t		ss_family;
	char			ss_pad1[6];
	int64_t			ss_align;
	char			ss_pad2[112];
};

struct sockaddr {
	sa_family_t		sa_family;
	char			sa_data[];
};

struct msghdr {
	void			*msg_name;
	socklen_t		msg_namelen;
	struct iovec	*msg_iov;
	int				msg_iovlen;
	void			*msg_control;
	socklen_t		msg_controllen;
	int				msg_flags;
};

struct cmsghdr {
	socklen_t	cmsg_len;
	int			cmsg_level;
	int			cmsg_type;
};

struct linger {
	int  l_onoff;
	int  l_linger;
};

int	accept(int, struct sockaddr *restrict, socklen_t *restrict);
int	bind(int, const struct sockaddr *, socklen_t);
int	connect(int, const struct sockaddr *, socklen_t);
int	getpeername(int, struct sockaddr *restrict, socklen_t *restrict);
int	getsockname(int, struct sockaddr *restrict, socklen_t *restrict);
int	getsockopt(int, int, int, void *restrict, socklen_t *restrict);
int	listen(int, int);
ssize_t recv(int, void *, size_t, int);
ssize_t recvfrom(int, void *restrict, size_t, int,
	struct sockaddr *restrict, socklen_t *restrict);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendmsg(int, const struct msghdr *, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
	socklen_t);
int	setsockopt(int, int, int, const void *, socklen_t);
int	shutdown(int, int);
int	socket(int, int, int);
int	sockatmark(int);
int	socketpair(int, int, int, int[2]);

#endif
