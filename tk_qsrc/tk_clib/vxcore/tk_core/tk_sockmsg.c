TK_SockDataMsg *tk_sockmsg_free[32];

TK_SockMsgQueue *tk_sockmsg_freesock;
TK_SockMsgQueue *tk_sockmsg_socket;
int tk_sockmsg_idrov;

byte *tk_sockmsg_portbmp_udp4;
byte *tk_sockmsg_portbmp_tcp4;
byte *tk_sockmsg_portbmp_udp6;
byte *tk_sockmsg_portbmp_tcp6;

int tk_sockmsg_portrov_udp4;
int tk_sockmsg_portrov_tcp4;
int tk_sockmsg_portrov_udp6;
int tk_sockmsg_portrov_tcp6;

int TK_SockMsg_IndexForMtuSize(int sz)
{
	int ix;
	if(sz<=960)
		return(0);
	ix=((TKMM_SizeToFxiU(sz+64)-0x38)+1)>>1;
	if(ix<0)
		ix=0;
	return(ix);
}

int TK_SockMsg_MtuSizeForIndex(int ix)
{
	return(TKMM_FxiToSize((ix<<1)+0x38)-64);
}

TK_SockDataMsg *TK_SockMsg_AllocMsg(int needsz)
{
	TK_SockDataMsg *msg;
	int ix, sz1;

	ix=TK_SockMsg_IndexForMtuSize(needsz);
	
	msg=tk_sockmsg_free[ix];
	if(msg)
	{
		tk_sockmsg_free[ix]=msg->next;
		return(msg);
	}
	
	sz1=TK_SockMsg_MtuSizeForIndex(ix);
	msg=tk_malloc(sz1+64);
	msg->sm_mtuix=ix;
	
	return(msg);
}

int TK_SockMsg_FreeMsg(TK_SockDataMsg *msg)
{
	int ix;
	
	ix=msg->sm_mtuix;
	msg->next=tk_sockmsg_free[ix];
	tk_sockmsg_free[ix]=msg;
	return(0);
}

TK_SockMsgQueue *TK_SockMsg_AllocSocket()
{
	TK_SockMsgQueue *sock;

	sock=tk_sockmsg_freesock;
	if(sock)
	{
		tk_sockmsg_freesock=sock->next;
		return(sock);
	}
	
	sock=tk_malloc(sizeof(TK_SockMsgQueue));
	return(sock);
}

int TK_SockMsg_FreeSocket(TK_SockMsgQueue *sock)
{
	sock->next=tk_sockmsg_freesock;
	tk_sockmsg_freesock=sock;
	return(0);
}


int TK_SockMsg_AllocPortBitmapBit(byte *bmp, int lo, int hi, int rov)
{
	int cur, base;
	
	cur=rov;
	if(cur<=0)
		cur=lo;
	while(cur>hi)
		cur-=hi;
	while(cur<lo)
		cur+=lo;
	base=cur;

	while(1)
	{
		if(!(bmp[cur>>3]&(1<<(cur&7))))
		{
			bmp[cur>>3]|=1<<(cur&7);
			return(cur);
		}
		if((bmp[cur>>3]==0xFF) && (cur>>3)!=(base>>3))
		{
			cur=(cur+8)&(~7);
			while(cur>hi)
				cur-=hi;
			while(cur<lo)
				cur+=lo;
			continue;
		}
		if((cur+1)==base)
			return(-1);
		cur++;
		while(cur>hi)
			cur-=hi;
		while(cur<lo)
			cur+=lo;
	}
	return(-1);
}

int TK_SockMsg_AllocPort(int proto)
{
	int pn;

	if(proto==TK_SOCKPROTO_UDP4)
	{
		if(!tk_sockmsg_portbmp_udp4)
		{
			tk_sockmsg_portbmp_udp4=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_udp4, 0, 8192);
		}
		
		pn=TK_SockMsg_AllocPortBitmapBit(tk_sockmsg_portbmp_udp4,
			4096, 65535, tk_sockmsg_portrov_udp4);
		tk_sockmsg_portrov_udp4=pn+1;
		return(pn);
	}

	if(proto==TK_SOCKPROTO_TCP4)
	{
		if(!tk_sockmsg_portbmp_tcp4)
		{
			tk_sockmsg_portbmp_tcp4=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_tcp4, 0, 8192);
		}
		
		pn=TK_SockMsg_AllocPortBitmapBit(tk_sockmsg_portbmp_tcp4,
			4096, 65535, tk_sockmsg_portrov_tcp4);
		tk_sockmsg_portrov_tcp4=pn+1;
		return(pn);
	}

	if(proto==TK_SOCKPROTO_UDP6)
	{
		if(!tk_sockmsg_portbmp_udp6)
		{
			tk_sockmsg_portbmp_udp6=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_udp6, 0, 8192);
		}
		
		pn=TK_SockMsg_AllocPortBitmapBit(tk_sockmsg_portbmp_udp6,
			4096, 65535, tk_sockmsg_portrov_udp6);
		tk_sockmsg_portrov_udp6=pn+1;
		return(pn);
	}

	if(proto==TK_SOCKPROTO_TCP6)
	{
		if(!tk_sockmsg_portbmp_tcp6)
		{
			tk_sockmsg_portbmp_tcp6=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_tcp6, 0, 8192);
		}
		
		pn=TK_SockMsg_AllocPortBitmapBit(tk_sockmsg_portbmp_tcp6,
			4096, 65535, tk_sockmsg_portrov_tcp6);
		tk_sockmsg_portrov_tcp6=pn+1;
		return(pn);
	}
	
	return(-1);
}

int TK_SockMsg_MarkPort(int proto, int port, int inuse)
{
	int pn;

	if(proto==TK_SOCKPROTO_UDP4)
	{
		if(!tk_sockmsg_portbmp_udp4)
		{
			tk_sockmsg_portbmp_udp4=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_udp4, 0, 8192);
		}		
		if(inuse)
			{ tk_sockmsg_portbmp_udp4[port>>3]|=(1<<(port&7)); }
		else
			{ tk_sockmsg_portbmp_udp4[port>>3]&=~(1<<(port&7)); }
		return(0);
	}

	if(proto==TK_SOCKPROTO_TCP4)
	{
		if(!tk_sockmsg_portbmp_tcp4)
		{
			tk_sockmsg_portbmp_tcp4=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_tcp4, 0, 8192);
		}
		
		if(inuse)
			{ tk_sockmsg_portbmp_tcp4[port>>3]|=(1<<(port&7)); }
		else
			{ tk_sockmsg_portbmp_tcp4[port>>3]&=~(1<<(port&7)); }
		return(0);
	}

	if(proto==TK_SOCKPROTO_UDP6)
	{
		if(!tk_sockmsg_portbmp_udp6)
		{
			tk_sockmsg_portbmp_udp6=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_udp6, 0, 8192);
		}
		
		if(inuse)
			{ tk_sockmsg_portbmp_udp6[port>>3]|=(1<<(port&7)); }
		else
			{ tk_sockmsg_portbmp_udp6[port>>3]&=~(1<<(port&7)); }
		return(0);
	}

	if(proto==TK_SOCKPROTO_TCP6)
	{
		if(!tk_sockmsg_portbmp_tcp6)
		{
			tk_sockmsg_portbmp_tcp6=tk_malloc(8192);
			memset(tk_sockmsg_portbmp_tcp6, 0, 8192);
		}
		
		if(inuse)
			{ tk_sockmsg_portbmp_tcp6[port>>3]|=(1<<(port&7)); }
		else
			{ tk_sockmsg_portbmp_tcp6[port>>3]&=~(1<<(port&7)); }
		return(0);
	}
	
	return(-1);
}

TK_SockMsgQueue *TK_SockMsg_CreateSocket(int proto)
{
	TK_SockMsgQueue *sock;
	
	sock=TK_SockMsg_AllocSocket();
	sock->sock_id=tk_sockmsg_idrov++;

	sock->sm_proto=proto;
	sock->src_port=TK_SockMsg_AllocPort(proto);

	if(	(proto==TK_SOCKPROTO_UDP4) ||
		(proto==TK_SOCKPROTO_TCP4))
	{
		sock->src_addr[0]=TK_SOCKV4_LOOPBACK;
		sock->src_addr[1]=0;
	}

	if(	(proto==TK_SOCKPROTO_UDP6) ||
		(proto==TK_SOCKPROTO_TCP6))
	{
		sock->src_addr[0]=1;
		sock->src_addr[1]=0;
	}

	sock->next=tk_sockmsg_socket;
	tk_sockmsg_socket=sock;
	
	return(sock);
}

TK_SockMsgQueue *TK_SockMsg_LookupSocketForAddr(int proto, u64 *addr, int port)
{
	TK_SockMsgQueue *cur;
	
	cur=tk_sockmsg_socket;
	while(cur)
	{
		if(	(cur->sm_proto==proto) &&
			(cur->src_port==port) &&
			(cur->src_addr[0]==addr[0]) &&
			(cur->src_addr[1]==addr[1]) )
		{
			return(cur);
		}
		cur=cur->next;
	}
	return(NULL);
}

TK_SockMsgQueue *TK_SockMsg_LookupSocketForID(int sock_id)
{
	TK_SockMsgQueue *cur;
	
	cur=tk_sockmsg_socket;
	while(cur)
	{
		if(cur->sock_id==sock_id)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

int TK_SockMsg_BindSocketPort(TK_SockMsgQueue *sock, int port)
{
	TK_SockMsg_MarkPort(sock->sm_proto, sock->src_port, 0);
	TK_SockMsg_MarkPort(sock->sm_proto, port, 1);
	sock->src_port=port;
}

int TK_SockMsg_DoRouteMsg(TK_SockDataMsg *msg)
{
	TK_SockMsgQueue *sock;
	
	sock=TK_SockMsg_LookupSocketForAddr(msg->sm_proto,
		msg->dst_addr, msg->dst_port);

	if(sock)
	{
		msg->next=sock->msg_recv;
		sock->msg_recv=msg;
		return(1);
	}
	
	/* Can't route for now. */
	TK_SockMsg_FreeMsg(msg);
	return(0);
}


int TK_SockMsg_SocketSendTo(TK_SockMsgQueue *sock,
	byte *msgbuf, int msglen,
	u64 *dst_addr, int dst_port)
{
	TK_SockDataMsg *msg;

	msg=TK_SockMsg_AllocMsg(msglen);

	msg->sm_proto=sock->sm_proto;
	msg->sm_size=msglen;

	msg->src_port=sock->src_port;
	msg->src_addr[0]=sock->src_addr[0];
	msg->src_addr[1]=sock->src_addr[1];

	msg->dst_port=dst_port;
	msg->dst_addr[0]=dst_addr[0];
	msg->dst_addr[1]=dst_addr[1];
	
	msg->sm_flags=0;
	msg->sm_seq=0;
	msg->sm_ack=0;
	
	memcpy(msg->sm_data, msgbuf, msglen);
	
	TK_SockMsg_DoRouteMsg(msg);
	return(0);
}
