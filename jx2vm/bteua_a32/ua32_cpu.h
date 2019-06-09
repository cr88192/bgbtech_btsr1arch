#define UA32_CC_EQ		0x0
#define UA32_CC_NE		0x1
#define UA32_CC_CS		0x2
#define UA32_CC_CC		0x3
#define UA32_CC_MI		0x4
#define UA32_CC_PL		0x5
#define UA32_CC_VS		0x6
#define UA32_CC_VC		0x7
#define UA32_CC_HI		0x8
#define UA32_CC_LS		0x9
#define UA32_CC_GE		0xA
#define UA32_CC_LT		0xB
#define UA32_CC_GT		0xC
#define UA32_CC_LE		0xD
#define UA32_CC_AL		0xE
#define UA32_CC_NV		0xF

// #define UA32_NMCC_EQ	0x0000
#define UA32_NMCC_AL	0x0000
#define UA32_NMCC_NE	0x1000
#define UA32_NMCC_CS	0x2000
#define UA32_NMCC_CC	0x3000
#define UA32_NMCC_MI	0x4000
#define UA32_NMCC_PL	0x5000
#define UA32_NMCC_VS	0x6000
#define UA32_NMCC_VC	0x7000
#define UA32_NMCC_HI	0x8000
#define UA32_NMCC_LS	0x9000
#define UA32_NMCC_GE	0xA000
#define UA32_NMCC_LT	0xB000
#define UA32_NMCC_GT	0xC000
#define UA32_NMCC_LE	0xD000
// #define UA32_NMCC_AL	0xE000
#define UA32_NMCC_EQ	0xE000
#define UA32_NMCC_NV	0xF000

#define UA32_NMCC_MASK	0xF000
#define UA32_NMCC_SHL	16

#define UA32_NMID_NONE		0x00
#define UA32_NMID_ADC		0x01
#define UA32_NMID_ADD		0x02
#define UA32_NMID_AND		0x03
#define UA32_NMID_B			0x04
#define UA32_NMID_BIC		0x05
#define UA32_NMID_BL		0x06
#define UA32_NMID_BX		0x07
#define UA32_NMID_CDP		0x08
#define UA32_NMID_CMN		0x09
#define UA32_NMID_CMP		0x0A
#define UA32_NMID_EOR		0x0B
#define UA32_NMID_LDC		0x0C
#define UA32_NMID_LDM		0x0D
#define UA32_NMID_LDR		0x0E
#define UA32_NMID_MCR		0x0F
#define UA32_NMID_MLA		0x10
#define UA32_NMID_MOV		0x11
#define UA32_NMID_MRC		0x12
#define UA32_NMID_MRS		0x13
#define UA32_NMID_MSR		0x14
#define UA32_NMID_MUL		0x15
#define UA32_NMID_MVM		0x16
#define UA32_NMID_ORR		0x17
#define UA32_NMID_RSB		0x18
#define UA32_NMID_RSC		0x19
#define UA32_NMID_SBC		0x1A
#define UA32_NMID_STC		0x1B
#define UA32_NMID_STM		0x1C
#define UA32_NMID_STR		0x1D
#define UA32_NMID_SUB		0x1E
#define UA32_NMID_SWI		0x1F
#define UA32_NMID_SWP		0x20
#define UA32_NMID_TEQ		0x21
#define UA32_NMID_TST		0x22

#define UA32_NMID_SMULL		0x24
#define UA32_NMID_SMLAL		0x25
#define UA32_NMID_UMULL		0x26
#define UA32_NMID_UMLAL		0x27

#define UA32_NMID_LDRB		0x28
#define UA32_NMID_LDRH		0x29
#define UA32_NMID_LDRSB		0x2A
#define UA32_NMID_LDRSH		0x2B
#define UA32_NMID_STRB		0x2C
#define UA32_NMID_STRH		0x2D

#define UA32_NMID_STRB		0x2C
#define UA32_NMID_STRH		0x2D

#define UA32_NMID_ADCS		0x81
#define UA32_NMID_ADDS		0x82
#define UA32_NMID_ANDS		0x83
#define UA32_NMID_BICS		0x85
#define UA32_NMID_CMNS		0x89
#define UA32_NMID_CMPS		0x8A
#define UA32_NMID_EORS		0x8B
#define UA32_NMID_POP		0x8E		//POP (Pseudo Op)
#define UA32_NMID_MLAS		0x90
#define UA32_NMID_MOVS		0x91
#define UA32_NMID_MULS		0x95
#define UA32_NMID_MVMS		0x96
#define UA32_NMID_ORRS		0x97
#define UA32_NMID_RSBS		0x98
#define UA32_NMID_RSCS		0x99
#define UA32_NMID_SBCS		0x9A
#define UA32_NMID_PUSH		0x9D		//PUSH (Pseudo Op)
#define UA32_NMID_SUBS		0x9E
#define UA32_NMID_TEQS		0xA1
#define UA32_NMID_TSTS		0xA2
#define UA32_NMID_SMULLS	0xA4
#define UA32_NMID_SMLALS	0xA5
#define UA32_NMID_UMULLS	0xA6
#define UA32_NMID_UMLALS	0xA7

#define UA32_NMID_MASK		0x0FF

#define UA32_NMPI_NONE	0x0000	//No Post-Increment
#define UA32_NMPI_IA	0x0600	//Post Increment
#define UA32_NMPI_DA	0x0200	//Post Decrement
#define UA32_NMPI_IB	0x0B00	//Pre Increment
#define UA32_NMPI_DB	0x0A00	//Pre Decrement

#define UA32_NMPI_MASK	0x0B00	//
#define UA32_NMPI_SHL	8		//


#define UA32_REG_R0		0x00
#define UA32_REG_R1		0x01
#define UA32_REG_R2		0x02
#define UA32_REG_R3		0x03
#define UA32_REG_R4		0x04
#define UA32_REG_R5		0x05
#define UA32_REG_R6		0x06
#define UA32_REG_R7		0x07
#define UA32_REG_R8		0x08
#define UA32_REG_R9		0x09
#define UA32_REG_R10	0x0A
#define UA32_REG_R11	0x0B
#define UA32_REG_R12	0x0C
#define UA32_REG_R13	0x0D
#define UA32_REG_R14	0x0E
#define UA32_REG_R15	0x0F

#define UA32_REG_SP		0x0D
#define UA32_REG_LR		0x0E
#define UA32_REG_PC		0x0F

#define UA32_SHL_LSL_I	0x00	//LSL Imm
#define UA32_SHL_LSR_I	0x40
#define UA32_SHL_ASR_I	0x80
#define UA32_SHL_ARR_I	0xC0

#define UA32_SHL_LSL_R	0x20	//LSL Reg
#define UA32_SHL_LSR_R	0x60
#define UA32_SHL_ASR_R	0xA0
#define UA32_SHL_ARR_R	0xE0


#define UA32_RLC_REL8	0x01		//
#define UA32_RLC_REL16	0x02		//
#define UA32_RLC_REL32	0x03		//
#define UA32_RLC_ABS8	0x04		//
#define UA32_RLC_ABS16	0x05		//
#define UA32_RLC_ABS32	0x06		//
#define UA32_RLC_ABS64	0x07		//

#define UA32_RLC_REL24W	0x08		//

#define UA32_CSEG_TEXT	0x00		//.text section
#define UA32_CSEG_DATA	0x01		//.data section
#define UA32_CSEG_BSS	0x02		//.bss section

#define UA32_CSEG_DYN	0x04		//dynamic sections

#define UA32_MAX_CACHEVAR	8


#define UA32_LBL_LOCALSTART	0xC000
#define UA32_LBL_NEAR		0x10000		//within +/- 128 bytes

#define UA32_MEMSC_TMASK	0xF0		//type mask
#define UA32_MEMSC_BYTE		0x10		//BYTE
#define UA32_MEMSC_WORD		0x20		//BYTE
#define UA32_MEMSC_DWORD	0x30		//BYTE
#define UA32_MEMSC_QWORD	0x40		//BYTE
#define UA32_MEMSC_XMM		0x50		//BYTE


typedef struct UA32_Context_s		UA32_Context;
typedef struct UA32_OpcodeArg_s	UA32_OpcodeArg;

struct UA32_Context_s {
byte *sec_buf[8];
byte *sec_end[8];
byte *sec_pos[8];
byte sec;
byte nsec;

int lbl_ofs[256];	//label offsets
int rlc_ofs[256];	//reloc offsets
u16 lbl_id[256];	//label IDs
u16 rlc_id[256];	//reloc label IDs
byte lbl_sec[256];	//label section
byte rlc_sec[256];	//reloc section
byte rlc_ty[256];	//reloc type
int nlbl;
int nrlc;
u16 lblrov;			//labels (local/temp)

byte reg_idx[UA32_MAX_CACHEVAR];
byte reg_reg[UA32_MAX_CACHEVAR];
int reg_live;
int reg_resv;
int reg_save;

int jitfl;
byte isjq;

UAX_Context *next;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct UA32_OpcodeArg_s {
byte sc;		//scale (1,2,3,4 for mem-ref)
byte ireg;		//index register
byte breg;		//base register
s64 disp;		//displacement or immed
int lbl;		//label
};

struct UA32_OpcodeDesc_s {
int id;
char *opstr;
char *args;
int next;
};
