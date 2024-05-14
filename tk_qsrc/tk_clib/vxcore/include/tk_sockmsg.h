/*
Internal socket message.
Used for UDP like sockets or bare IP datagrams.
 */

#define TK_SOCKPROTO_UDP4	4
#define TK_SOCKPROTO_TCP4	5
#define TK_SOCKPROTO_UDP6	6
#define TK_SOCKPROTO_TCP6	7

#define TK_SOCKV4_LOOPBACK	0x7F000001ULL

typedef struct TK_SockDataMsg_s TK_SockDataMsg;
typedef struct TK_SockMsgQueue_s TK_SockMsgQueue;

struct TK_SockDataMsg_s {
TK_SockDataMsg *next;	//00
u16 src_port;			//08
u16 dst_port;			//0A
u16 sm_size;			//0C
byte sm_proto;			//0E
byte sm_pad2;			//0F
u64 src_addr[2];		//10
u64 dst_addr[2];		//20
u32 sm_pad0;			//30
byte sm_pad1;			//34
byte sm_mtuix;			//35
u16 sm_flags;			//36
u32 sm_seq;				//38
u32 sm_ack;				//3C
byte sm_data[960];		//40, data area, variable
};

struct TK_SockMsgQueue_s {
TK_SockMsgQueue *next;
TK_SockDataMsg *msg_send;	//send queue
TK_SockDataMsg *msg_recv;	//recieve queue
u16 src_port;				//local port
u16 dst_port;				//remote port
byte sm_proto;				//
u64 src_addr[2];			//
u64 dst_addr[2];			//
int sock_id;
};
