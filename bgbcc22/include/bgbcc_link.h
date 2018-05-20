#ifndef BGBCC_LINK_H
#define BGBCC_LINK_H

#define BGBCC_CPU_X86		1
#define BGBCC_CPU_X64		2
#define BGBCC_CPU_ARM		3
#define BGBCC_CPU_THUMB		4

#define BGBCC_JMP_SHORT		1
#define BGBCC_JMP_NEAR16		2
#define BGBCC_JMP_NEAR32		3
#define BGBCC_JMP_ABS16		4
#define BGBCC_JMP_ABS32		5
#define BGBCC_JMP_ABS64		6
#define BGBCC_JMP_ABS		7
#define BGBCC_JMP_ABS32NB	8
#define BGBCC_JMP_SECTION	9
#define BGBCC_JMP_SECREL		10

#define BGBCC_JMP_ARM_NEAR24			16
#define BGBCC_JMP_THUMB_NEAR8		17
#define BGBCC_JMP_THUMB_NEAR11		18
#define BGBCC_JMP_THUMB_NEAR22		19

#define BGBCC_FL_X86_64		1
#define BGBCC_FL_X86_16		2
#define BGBCC_FL_DATA		4
#define BGBCC_FL_ADDRSZ		8
#define BGBCC_FL_GAS			16
#define BGBCC_FL_ARM			32
#define BGBCC_FL_THUMB		64

typedef struct BGBCC_Object_s BGBCC_Object;

struct BGBCC_Object_s {
BGBCC_Object *next;
char *name;

char **label_name;
int *label_pos;
int n_labels, m_labels;

char **goto_name;
int *goto_pos;
byte *goto_type;
int n_gotos, m_gotos;

char **proxy_name;
int n_proxy, m_proxy;

byte *text, *data;
int text_sz, data_sz;
int flags;
};

typedef struct BGBCC_COFF_Info_s BGBCC_COFF_Info;

struct BGBCC_COFF_Info_s
{
BGBCC_COFF_Info *next;		//next loaded object
BGBCC_COFF_Info *tnext;		//temporary next link

char *name;	//object file name
byte *buf;	//object file buffer
int bsz;	//object fize size

byte *text;	//location of in-memory code
byte *data;	//location of in-memory data (data, bss, ...)
int tsz, dsz;	//size of in-memory code/data

int peoffs, coff_hdr, symoffs, nsyms;
int secoffs, nsecs, stroffs;


char *sec_name[16];
int sec_flag[16];

int sec_rva_off[16];
int sec_rva_sz[16];
int sec_buf_off[16];
int sec_buf_sz[16];
int sec_rlc_off[16];
int sec_rlc_num[16];

char **sym_name;	//symbol name
void **sym_addr;	//symbol address, internal or external
int *sym_offs;		//offset of symbol
int *sym_sec;		//section for symbol
byte *sym_stype;		//storage type for symbol
};

#endif
