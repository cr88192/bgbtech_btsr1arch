#ifndef __TK_SYS_UN_H
#define __TK_SYS_UN_H

#include <sys/socket.h>

struct sockaddr_un {
	sa_family_t		sun_family;		/* Address family */
	char			sun_path[126];	/* Socket pathname */
};

#endif
