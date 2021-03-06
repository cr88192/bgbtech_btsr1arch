
#define TK_UMSG_ROMFCN			0x1000

#define TK_UMSG_GETCONPARAM		0x1001
#define TK_UMSG_CONRESET		0x1002
#define TK_UMSG_PGMEXIT			0x1003
#define TK_UMSG_PAGEALLOC		0x1004
#define TK_UMSG_PAGEFREE		0x1005
#define TK_UMSG_SETUSRIRQ		0x1006

#define TK_UMSG_CONPUTC			0x1008
#define TK_UMSG_CONGETCH		0x1009
#define TK_UMSG_CONKBHIT		0x100A
#define TK_UMSG_GETENVVARIDX	0x100B
#define TK_UMSG_GETENVVAR		0x100C
#define TK_UMSG_SETENVVAR		0x100D

#define TK_UMSG_MMAP			0x1010
#define TK_UMSG_MUNMAP			0x1011
#define TK_UMSG_MPROTECT		0x1012
#define TK_UMSG_MSYNC			0x1013
#define TK_UMSG_MLOCK			0x1014
#define TK_UMSG_MUNLOCK			0x1015
#define TK_UMSG_MLOCKALL		0x1016
#define TK_UMSG_MUNLOCKALL		0x1017

#define TK_UMSG_VFOPEN			0x1020
#define TK_UMSG_VFOPENDIR		0x1021
#define TK_UMSG_VFUNLINK		0x1022
#define TK_UMSG_VFRENAME		0x1023
#define TK_UMSG_VFSTAT			0x1024
#define TK_UMSG_VFMOUNT			0x1025
#define TK_UMSG_VFREADDIR		0x1026
#define TK_UMSG_VFCLOSEDIR		0x1027
#define TK_UMSG_VFREAD			0x1028
#define TK_UMSG_VFWRITE			0x1029
#define TK_UMSG_VFSEEK			0x102A
#define TK_UMSG_VFCLOSE			0x102B
#define TK_UMSG_VFIOCTL			0x102C
#define TK_UMSG_VFSCTL			0x102D
#define TK_UMSG_VFMKDIR			0x102E
#define TK_UMSG_VFRMDIR			0x102F
#define TK_UMSG_VFSEND			0x1030
#define TK_UMSG_VFRECV			0x1031

// #define TK_UMSG_VFTELL			0x102B
// #define TK_UMSG_VFGETC			0x102D
// #define TK_UMSG_VFPUTC			0x102E

#define TK_URES_FALSE			0x0000
#define TK_URES_TRUE			0x0001

#define TK_URES_ERR_UNKNOWN		0xFFFF
#define TK_URES_ERR_BADMSG		0xFFF0

#define TK_IOC_SEND				0x4000
#define TK_IOC_RECV				0x4001
#define TK_IOC_SENDTO			0x4002
#define TK_IOC_RECVFROM			0x4003
#define TK_IOC_SENDMSG			0x4004
#define TK_IOC_RECVMSG			0x4005

#define TK_IOC_VFB_BLIT			0x4101
#define TK_IOC_VFB_COPYRECT		0x4102

typedef union TK_SysArg_s TK_SysArg;
union TK_SysArg_s {
s32 i;
u32 ui;
s64 l;
u64 ul;
void *p;
};
