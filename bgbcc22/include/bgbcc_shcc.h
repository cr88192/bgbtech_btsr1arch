/*
 * SH-ISA
 */

/* 00-1F: GPR R00-R31, 32/64 */
#define BGBCC_SH_REG_R0			0x00
#define BGBCC_SH_REG_R1			0x01
#define BGBCC_SH_REG_R2			0x02
#define BGBCC_SH_REG_R3			0x03
#define BGBCC_SH_REG_R4			0x04
#define BGBCC_SH_REG_R5			0x05
#define BGBCC_SH_REG_R6			0x06
#define BGBCC_SH_REG_R7			0x07
#define BGBCC_SH_REG_R8			0x08
#define BGBCC_SH_REG_R9			0x09
#define BGBCC_SH_REG_R10		0x0A
#define BGBCC_SH_REG_R11		0x0B
#define BGBCC_SH_REG_R12		0x0C
#define BGBCC_SH_REG_R13		0x0D
#define BGBCC_SH_REG_R14		0x0E
#define BGBCC_SH_REG_R15		0x0F
#define BGBCC_SH_REG_R16		0x10
#define BGBCC_SH_REG_R17		0x11
#define BGBCC_SH_REG_R18		0x12
#define BGBCC_SH_REG_R19		0x13
#define BGBCC_SH_REG_R20		0x14
#define BGBCC_SH_REG_R21		0x15
#define BGBCC_SH_REG_R22		0x16
#define BGBCC_SH_REG_R23		0x17
#define BGBCC_SH_REG_R24		0x18	
#define BGBCC_SH_REG_R25		0x19
#define BGBCC_SH_REG_R26		0x1A
#define BGBCC_SH_REG_R27		0x1B
#define BGBCC_SH_REG_R28		0x1C
#define BGBCC_SH_REG_R29		0x1D
#define BGBCC_SH_REG_R30		0x1E
#define BGBCC_SH_REG_R31		0x1F

#define BGBCC_SH_REG_BP			0x0E
#define BGBCC_SH_REG_SP			0x0F


/* 20-3F: GPR RQ00-RQ31, 64 */
#define BGBCC_SH_REG_RQ0		0x20
#define BGBCC_SH_REG_RQ1		0x21
#define BGBCC_SH_REG_RQ2		0x22
#define BGBCC_SH_REG_RQ3		0x23
#define BGBCC_SH_REG_RQ4		0x24
#define BGBCC_SH_REG_RQ5		0x25
#define BGBCC_SH_REG_RQ6		0x26
#define BGBCC_SH_REG_RQ7		0x27
#define BGBCC_SH_REG_RQ8		0x28
#define BGBCC_SH_REG_RQ9		0x29
#define BGBCC_SH_REG_RQ10		0x2A
#define BGBCC_SH_REG_RQ11		0x2B
#define BGBCC_SH_REG_RQ12		0x2C
#define BGBCC_SH_REG_RQ13		0x2D
#define BGBCC_SH_REG_RQ14		0x2E
#define BGBCC_SH_REG_RQ15		0x2F
#define BGBCC_SH_REG_RQ16		0x30
#define BGBCC_SH_REG_RQ17		0x31
#define BGBCC_SH_REG_RQ18		0x32
#define BGBCC_SH_REG_RQ19		0x33
#define BGBCC_SH_REG_RQ20		0x34
#define BGBCC_SH_REG_RQ21		0x35
#define BGBCC_SH_REG_RQ22		0x36
#define BGBCC_SH_REG_RQ23		0x37
#define BGBCC_SH_REG_RQ24		0x38	
#define BGBCC_SH_REG_RQ25		0x39
#define BGBCC_SH_REG_RQ26		0x3A
#define BGBCC_SH_REG_RQ27		0x3B
#define BGBCC_SH_REG_RQ28		0x3C
#define BGBCC_SH_REG_RQ29		0x3D
#define BGBCC_SH_REG_RQ30		0x3E
#define BGBCC_SH_REG_RQ31		0x3F

/* 40-5F: GPR RD00-RD31, 32 */
#define BGBCC_SH_REG_RD0		0x40
#define BGBCC_SH_REG_RD1		0x41
#define BGBCC_SH_REG_RD2		0x42
#define BGBCC_SH_REG_RD3		0x43
#define BGBCC_SH_REG_RD4		0x44
#define BGBCC_SH_REG_RD5		0x45
#define BGBCC_SH_REG_RD6		0x46
#define BGBCC_SH_REG_RD7		0x47
#define BGBCC_SH_REG_RD8		0x48
#define BGBCC_SH_REG_RD9		0x49
#define BGBCC_SH_REG_RD10		0x4A
#define BGBCC_SH_REG_RD11		0x4B
#define BGBCC_SH_REG_RD12		0x4C
#define BGBCC_SH_REG_RD13		0x4D
#define BGBCC_SH_REG_RD14		0x4E
#define BGBCC_SH_REG_RD15		0x4F
#define BGBCC_SH_REG_RD16		0x50
#define BGBCC_SH_REG_RD17		0x51
#define BGBCC_SH_REG_RD18		0x52
#define BGBCC_SH_REG_RD19		0x53
#define BGBCC_SH_REG_RD20		0x54
#define BGBCC_SH_REG_RD21		0x55
#define BGBCC_SH_REG_RD22		0x56
#define BGBCC_SH_REG_RD23		0x57
#define BGBCC_SH_REG_RD24		0x58	
#define BGBCC_SH_REG_RD25		0x59
#define BGBCC_SH_REG_RD26		0x5A
#define BGBCC_SH_REG_RD27		0x5B
#define BGBCC_SH_REG_RD28		0x5C
#define BGBCC_SH_REG_RD29		0x5D
#define BGBCC_SH_REG_RD30		0x5E
#define BGBCC_SH_REG_RD31		0x5F

/* 60-6F: Control Regs */
#define BGBCC_SH_REG_CR0		0x60

#define BGBCC_SH_REG_SR			0x60
#define BGBCC_SH_REG_GBR		0x61
#define BGBCC_SH_REG_VBR		0x62
#define BGBCC_SH_REG_SSR		0x63
#define BGBCC_SH_REG_SPC		0x64

#define BGBCC_SH_REG_R0B		0x68
#define BGBCC_SH_REG_R1B		0x69
#define BGBCC_SH_REG_R2B		0x6A
#define BGBCC_SH_REG_R3B		0x6B
#define BGBCC_SH_REG_R4B		0x6C
#define BGBCC_SH_REG_R5B		0x6D
#define BGBCC_SH_REG_R6B		0x6E
#define BGBCC_SH_REG_R7B		0x6F

/* 70-7F: Status Regs */
#define BGBCC_SH_REG_SR0		0x70

#define BGBCC_SH_REG_MACH		0x70	//S
#define BGBCC_SH_REG_MACL		0x71	//S
#define BGBCC_SH_REG_PR			0x72	//S
#define BGBCC_SH_REG_SGR		0x73	//C

#define BGBCC_SH_REG_FPUL		0x75
#define BGBCC_SH_REG_FPSCR		0x76
#define BGBCC_SH_REG_PC			0x77

#define BGBCC_SH_REG_DBR		0x7F

/* 80-9F: FPR DR0-DR15, 64 */
#define BGBCC_SH_REG_DR0		0x80
#define BGBCC_SH_REG_DR1		0x81
#define BGBCC_SH_REG_DR2		0x82
#define BGBCC_SH_REG_DR3		0x83
#define BGBCC_SH_REG_DR4		0x84
#define BGBCC_SH_REG_DR5		0x85
#define BGBCC_SH_REG_DR6		0x86
#define BGBCC_SH_REG_DR7		0x87
#define BGBCC_SH_REG_DR8		0x88	
#define BGBCC_SH_REG_DR9		0x89
#define BGBCC_SH_REG_DR10		0x8A
#define BGBCC_SH_REG_DR11		0x8B
#define BGBCC_SH_REG_DR12		0x8C
#define BGBCC_SH_REG_DR13		0x8D
#define BGBCC_SH_REG_DR14		0x8E
#define BGBCC_SH_REG_DR15		0x8F
#define BGBCC_SH_REG_DR16		0x90
#define BGBCC_SH_REG_DR17		0x91
#define BGBCC_SH_REG_DR18		0x92
#define BGBCC_SH_REG_DR19		0x93
#define BGBCC_SH_REG_DR20		0x94
#define BGBCC_SH_REG_DR21		0x95
#define BGBCC_SH_REG_DR22		0x96
#define BGBCC_SH_REG_DR23		0x97
#define BGBCC_SH_REG_DR24		0x98	
#define BGBCC_SH_REG_DR25		0x99
#define BGBCC_SH_REG_DR26		0x9A
#define BGBCC_SH_REG_DR27		0x9B
#define BGBCC_SH_REG_DR28		0x9C
#define BGBCC_SH_REG_DR29		0x9D
#define BGBCC_SH_REG_DR30		0x9E
#define BGBCC_SH_REG_DR31		0x9F

/* A0-AF: FPR FR0-FR15, 32 */
#define BGBCC_SH_REG_FR0		0xA0
#define BGBCC_SH_REG_FR1		0xA1
#define BGBCC_SH_REG_FR2		0xA2
#define BGBCC_SH_REG_FR3		0xA3
#define BGBCC_SH_REG_FR4		0xA4
#define BGBCC_SH_REG_FR5		0xA5
#define BGBCC_SH_REG_FR6		0xA6
#define BGBCC_SH_REG_FR7		0xA7
#define BGBCC_SH_REG_FR8		0xA8
#define BGBCC_SH_REG_FR9		0xA9
#define BGBCC_SH_REG_FR10		0xAA
#define BGBCC_SH_REG_FR11		0xAB
#define BGBCC_SH_REG_FR12		0xAC
#define BGBCC_SH_REG_FR13		0xAD
#define BGBCC_SH_REG_FR14		0xAE
#define BGBCC_SH_REG_FR15		0xAF

/* B0-BF: FPR XF0-XF15, 32 */
#define BGBCC_SH_REG_XF0		0xB0
#define BGBCC_SH_REG_XF1		0xB1
#define BGBCC_SH_REG_XF2		0xB2
#define BGBCC_SH_REG_XF3		0xB3
#define BGBCC_SH_REG_XF4		0xB4
#define BGBCC_SH_REG_XF12		0xBC
#define BGBCC_SH_REG_XF13		0xBD
#define BGBCC_SH_REG_XF14		0xBE
#define BGBCC_SH_REG_XF15		0xBF

/* C0-CF: FPR YF0-YF15, 32 */
#define BGBCC_SH_REG_YF0		0xC0
#define BGBCC_SH_REG_YF1		0xC1
#define BGBCC_SH_REG_YF2		0xC2
#define BGBCC_SH_REG_YF3		0xC3
#define BGBCC_SH_REG_YF4		0xC4
#define BGBCC_SH_REG_YF12		0xCC
#define BGBCC_SH_REG_YF13		0xCD
#define BGBCC_SH_REG_YF14		0xCE
#define BGBCC_SH_REG_YF15		0xCF

/* D0-DF: FPR ZF0-ZF15, 32 */
#define BGBCC_SH_REG_ZF0		0xD0
#define BGBCC_SH_REG_ZF1		0xD1
#define BGBCC_SH_REG_ZF2		0xD2
#define BGBCC_SH_REG_ZF3		0xD3
#define BGBCC_SH_REG_ZF4		0xD4
#define BGBCC_SH_REG_ZF12		0xDC
#define BGBCC_SH_REG_ZF13		0xDD
#define BGBCC_SH_REG_ZF14		0xDE
#define BGBCC_SH_REG_ZF15		0xDF

#if 0
/* E0-EF: GPR R0-R15 High Bits, 32 */
#define BGBCC_SH_REG_RH0		0xE0
#define BGBCC_SH_REG_RH1		0xE1
#define BGBCC_SH_REG_RH2		0xE2
#define BGBCC_SH_REG_RH3		0xE3
#define BGBCC_SH_REG_RH4		0xE4
#define BGBCC_SH_REG_RH5		0xE5
#define BGBCC_SH_REG_RH6		0xE6
#define BGBCC_SH_REG_RH7		0xE7
#define BGBCC_SH_REG_RH8		0xE8
#define BGBCC_SH_REG_RH9		0xE9
#define BGBCC_SH_REG_RH10		0xEA
#define BGBCC_SH_REG_RH11		0xEB
#define BGBCC_SH_REG_RH12		0xEC
#define BGBCC_SH_REG_RH13		0xED
#define BGBCC_SH_REG_RH14		0xEE
#define BGBCC_SH_REG_RH15		0xEF

/* F0-FF: GPR R0-R15 Pair, 32+32 */
#define BGBCC_SH_REG_LR0		0xF0
#define BGBCC_SH_REG_LR1		0xF1
#define BGBCC_SH_REG_LR2		0xF2
#define BGBCC_SH_REG_LR3		0xF3
#define BGBCC_SH_REG_LR4		0xF4
#define BGBCC_SH_REG_LR5		0xF5
#define BGBCC_SH_REG_LR6		0xF6
#define BGBCC_SH_REG_LR7		0xF7
#define BGBCC_SH_REG_LR8		0xF8
#define BGBCC_SH_REG_LR9		0xF9
#define BGBCC_SH_REG_LR10		0xFA
#define BGBCC_SH_REG_LR11		0xFB
#define BGBCC_SH_REG_LR12		0xFC
#define BGBCC_SH_REG_LR13		0xFD
#define BGBCC_SH_REG_LR14		0xFE
#define BGBCC_SH_REG_LR15		0xFF		//Invalid

#endif

#if 1
/* E0-EF: GPR R0-R15 Pair */
#define BGBCC_SH_REG_LR0		0xE0
#define BGBCC_SH_REG_LR1		0xE1
#define BGBCC_SH_REG_LR2		0xE2
#define BGBCC_SH_REG_LR3		0xE3
#define BGBCC_SH_REG_LR4		0xE4
#define BGBCC_SH_REG_LR5		0xE5
#define BGBCC_SH_REG_LR6		0xE6
#define BGBCC_SH_REG_LR7		0xE7
#define BGBCC_SH_REG_LR8		0xE8
#define BGBCC_SH_REG_LR9		0xE9
#define BGBCC_SH_REG_LR10		0xEA
#define BGBCC_SH_REG_LR11		0xEB
#define BGBCC_SH_REG_LR12		0xEC
#define BGBCC_SH_REG_LR13		0xED
#define BGBCC_SH_REG_LR14		0xEE
#define BGBCC_SH_REG_LR15		0xEF		//Invalid

/* F0-FF: GPR R16-R31 Pair */
#define BGBCC_SH_REG_LR16		0xF0
#define BGBCC_SH_REG_LR17		0xF1
#define BGBCC_SH_REG_LR18		0xF2
#define BGBCC_SH_REG_LR19		0xF3
#define BGBCC_SH_REG_LR20		0xF4
#define BGBCC_SH_REG_LR21		0xF5
#define BGBCC_SH_REG_LR22		0xF6
#define BGBCC_SH_REG_LR23		0xF7
#define BGBCC_SH_REG_LR24		0xF8
#define BGBCC_SH_REG_LR25		0xF9
#define BGBCC_SH_REG_LR26		0xFA
#define BGBCC_SH_REG_LR27		0xFB
#define BGBCC_SH_REG_LR28		0xFC
#define BGBCC_SH_REG_LR29		0xFD
#define BGBCC_SH_REG_LR30		0xFE
#define BGBCC_SH_REG_LR31		0xFF		//Invalid

#endif

#define BGBCC_SH_REG_RTMASK		0xF0
#define BGBCC_SH_REG_RTMASK3	0xF8
#define BGBCC_SH_REG_RTMASK5	0xE0

#define BGBCC_SH_REG_ZZR		0xFF		//Null Register (Placeholder Reg)


#define BGBCC_SH_REG_DLR	BGBCC_SH_REG_MACL	//Alias
#define BGBCC_SH_REG_DHR	BGBCC_SH_REG_MACH	//Alias


#define BGBCC_SH_NMID_UNK			0x00	//Unknown
#define BGBCC_SH_NMID_MOV			0x01	//MOV
#define BGBCC_SH_NMID_MOVB			0x02	//MOV.B
#define BGBCC_SH_NMID_MOVW			0x03	//MOV.W
#define BGBCC_SH_NMID_MOVL			0x04	//MOV.L
#define BGBCC_SH_NMID_ADD			0x05	//ADD
#define BGBCC_SH_NMID_ADDC			0x06	//ADDC
#define BGBCC_SH_NMID_ADDV			0x07	//ADDV
#define BGBCC_SH_NMID_SUB			0x08	//SUB
#define BGBCC_SH_NMID_SUBC			0x09	//SUBC
#define BGBCC_SH_NMID_SUBV			0x0A	//SUBV
#define BGBCC_SH_NMID_CMPHS			0x0B	//CMPHS
#define BGBCC_SH_NMID_CMPHI			0x0C	//CMPHI
#define BGBCC_SH_NMID_CMPEQ			0x0D	//CMPEQ
#define BGBCC_SH_NMID_CMPGE			0x0E	//CMPGE
#define BGBCC_SH_NMID_CMPGT			0x0F	//CMPGT
#define BGBCC_SH_NMID_JMP			0x10	//JMP
#define BGBCC_SH_NMID_JSR			0x11	//JSR
#define BGBCC_SH_NMID_BRA			0x12	//BRA
#define BGBCC_SH_NMID_BSR			0x13	//BSR
#define BGBCC_SH_NMID_BT			0x14	//BT
#define BGBCC_SH_NMID_BF			0x15	//BF
#define BGBCC_SH_NMID_BTS			0x16	//BTS
#define BGBCC_SH_NMID_BFS			0x17	//BFS
#define BGBCC_SH_NMID_DIV1			0x18	//DIV1
#define BGBCC_SH_NMID_DMULU			0x19	//DMULU
#define BGBCC_SH_NMID_DMULS			0x1A	//DMULS
#define BGBCC_SH_NMID_TST			0x1B	//TST
#define BGBCC_SH_NMID_AND			0x1C	//AND
#define BGBCC_SH_NMID_XOR			0x1D	//XOR
#define BGBCC_SH_NMID_OR			0x1E	//OR
#define BGBCC_SH_NMID_INVALID		0x1F	//Invalid
#define BGBCC_SH_NMID_DIV0S			0x20	//
#define BGBCC_SH_NMID_CMPSTR		0x21	//
#define BGBCC_SH_NMID_MULUW			0x22	//
#define BGBCC_SH_NMID_MULSW			0x23	//
#define BGBCC_SH_NMID_MACL			0x24	//
#define BGBCC_SH_NMID_BSRF			0x25	//
#define BGBCC_SH_NMID_BRAF			0x26	//
#define BGBCC_SH_NMID_MULL			0x27	//
#define BGBCC_SH_NMID_CLRT			0x28	//
#define BGBCC_SH_NMID_SETT			0x29	//
#define BGBCC_SH_NMID_CLRMAC		0x2A	//
#define BGBCC_SH_NMID_NOP			0x2B	//
#define BGBCC_SH_NMID_MOVT			0x2C	//
#define BGBCC_SH_NMID_RTS			0x2D	//
#define BGBCC_SH_NMID_SLEEP			0x2E	//
#define BGBCC_SH_NMID_RTE			0x2F	//
#define BGBCC_SH_NMID_SHLL			0x30	//
#define BGBCC_SH_NMID_DT			0x31	//
#define BGBCC_SH_NMID_SHAL			0x32	//
#define BGBCC_SH_NMID_SHLR			0x33	//
#define BGBCC_SH_NMID_CMPPZ			0x34	//
#define BGBCC_SH_NMID_SHAR			0x35	//
#define BGBCC_SH_NMID_ROTL			0x36	//
#define BGBCC_SH_NMID_ROTCL			0x37	//
#define BGBCC_SH_NMID_ROTT			0x38	//
#define BGBCC_SH_NMID_ROTCR			0x39	//
#define BGBCC_SH_NMID_SHLL2			0x3A	//
#define BGBCC_SH_NMID_ROTR			0x3B	//
#define BGBCC_SH_NMID_CMPPL			0x3C	//
#define BGBCC_SH_NMID_SHLL8			0x3D	//
#define BGBCC_SH_NMID_SHLL16		0x3E	//
#define BGBCC_SH_NMID_SHLR2			0x3F	//
#define BGBCC_SH_NMID_SHLR8			0x40	//
#define BGBCC_SH_NMID_SHLR16		0x41	//
#define BGBCC_SH_NMID_MACW			0x42	//
#define BGBCC_SH_NMID_NOT			0x43	//
#define BGBCC_SH_NMID_SWAPB			0x44	//
#define BGBCC_SH_NMID_SWAPW			0x45	//
#define BGBCC_SH_NMID_NEGC			0x46	//
#define BGBCC_SH_NMID_NEG			0x47	//
#define BGBCC_SH_NMID_EXTUB			0x48	//
#define BGBCC_SH_NMID_EXTUW			0x49	//
#define BGBCC_SH_NMID_EXTSB			0x4A	//
#define BGBCC_SH_NMID_EXTSW			0x4B	//
#define BGBCC_SH_NMID_TRAPA			0x4C	//
#define BGBCC_SH_NMID_MOVA			0x4D	//
#define BGBCC_SH_NMID_TSTB			0x4E	//
#define BGBCC_SH_NMID_DIV0U			0x4F	//
#define BGBCC_SH_NMID_LDC			0x50	//
#define BGBCC_SH_NMID_LDCL			0x51	//
#define BGBCC_SH_NMID_LDS			0x52	//
#define BGBCC_SH_NMID_LDSL			0x53	//
#define BGBCC_SH_NMID_STC			0x54	//
#define BGBCC_SH_NMID_STCL			0x55	//
#define BGBCC_SH_NMID_STS			0x56	//
#define BGBCC_SH_NMID_STSL			0x57	//
#define BGBCC_SH_NMID_CASL			0x58	//
#define BGBCC_SH_NMID_TASB			0x59	//
#define BGBCC_SH_NMID_XTRCT			0x5A	//
#define BGBCC_SH_NMID_ANDB			0x5B	//
#define BGBCC_SH_NMID_XORB			0x5C	//
#define BGBCC_SH_NMID_ORB			0x5D	//
#define BGBCC_SH_NMID_SHAD			0x5E	//
#define BGBCC_SH_NMID_SHLD			0x5F	//
#define BGBCC_SH_NMID_INVDLY		0x60	//
#define BGBCC_SH_NMID_CLRS			0x61	//
#define BGBCC_SH_NMID_SETS			0x62	//
#define BGBCC_SH_NMID_MOVI20		0x63	//MOVI20
#define BGBCC_SH_NMID_MOVI20S		0x64	//MOVI20S
#define BGBCC_SH_NMID_MOVUB			0x65	//MOVU.B
#define BGBCC_SH_NMID_MOVUW			0x66	//MOVU.W
#define BGBCC_SH_NMID_MOVUL			0x67	//MOVU.L
#define BGBCC_SH_NMID_LEAB			0x68	//LEA.B
#define BGBCC_SH_NMID_LEAW			0x69	//LEA.W
#define BGBCC_SH_NMID_LEAL			0x6A	//LEA.L
#define BGBCC_SH_NMID_LEAQ			0x6B	//LEA.Q
#define BGBCC_SH_NMID_LEAO			0x6C	//LEA.O
#define BGBCC_SH_NMID_NOTT			0x6D	//
#define BGBCC_SH_NMID_LDTLB			0x6E	//
#define BGBCC_SH_NMID_BRK			0x6F	//
#define BGBCC_SH_NMID_MOVRT			0x70	//
#define BGBCC_SH_NMID_MOVCAL		0x71	//

#define BGBCC_SH_NMID_PUSH			0x72	//
#define BGBCC_SH_NMID_POP			0x73	//
#define BGBCC_SH_NMID_ICLRMD		0x74	//
#define BGBCC_SH_NMID_ISETMD		0x75	//
#define BGBCC_SH_NMID_SHLL1			0x76	//
#define BGBCC_SH_NMID_LDHF16		0x77	//
#define BGBCC_SH_NMID_BRAN			0x78	//BRA/N
#define BGBCC_SH_NMID_BSRN			0x79	//BSR/N
#define BGBCC_SH_NMID_RTSN			0x7A	//
#define BGBCC_SH_NMID_LDIF16		0x7B	//
#define BGBCC_SH_NMID_STHF16		0x7C	//
#define BGBCC_SH_NMID_CSELT			0x7D	//
#define BGBCC_SH_NMID_EXTUL			0x7E	//
#define BGBCC_SH_NMID_EXTSL			0x7F	//

#define BGBCC_SH_NMID_FABS			0x80	//
#define BGBCC_SH_NMID_FADD			0x81	//
#define BGBCC_SH_NMID_FCMPEQ		0x82	//
#define BGBCC_SH_NMID_FCMPGT		0x83	//
#define BGBCC_SH_NMID_FCNVDS		0x84	//
#define BGBCC_SH_NMID_FCNVSD		0x85	//
#define BGBCC_SH_NMID_FDIV			0x86	//
#define BGBCC_SH_NMID_FLDI0			0x87	//
#define BGBCC_SH_NMID_FLDI1			0x88	//
#define BGBCC_SH_NMID_FLDS			0x89	//
#define BGBCC_SH_NMID_FLOAT			0x8A	//
#define BGBCC_SH_NMID_FMAC			0x8B	//
#define BGBCC_SH_NMID_FMOV			0x8C	//
#define BGBCC_SH_NMID_FMOVS			0x8D	//
#define BGBCC_SH_NMID_FMOVD			0x8E	//
#define BGBCC_SH_NMID_FMUL			0x8F	//
#define BGBCC_SH_NMID_FNEG			0x90	//
#define BGBCC_SH_NMID_FSCHG			0x91	//
#define BGBCC_SH_NMID_FSQRT			0x92	//
#define BGBCC_SH_NMID_FSTS			0x93	//
#define BGBCC_SH_NMID_FSUB			0x94	//
#define BGBCC_SH_NMID_FTRC			0x95	//
#define BGBCC_SH_NMID_FSRRA			0x96	//
#define BGBCC_SH_NMID_FIPR			0x97	//
#define BGBCC_SH_NMID_FTRV			0x98	//
#define BGBCC_SH_NMID_PSETMD4		0x99	//
#define BGBCC_SH_NMID_PSETMD12		0x9A	//
#define BGBCC_SH_NMID_FMOVX			0x9B	//
#define BGBCC_SH_NMID_FMOVIS		0x9C	//
#define BGBCC_SH_NMID_FMOVID		0x9D	//
#define BGBCC_SH_NMID_FMOVSI		0x9E	//
#define BGBCC_SH_NMID_FMOVDI		0x9F	//

#define BGBCC_SH_NMID_CMOVTB		0xA0	//CMOVT.B
#define BGBCC_SH_NMID_CMOVTW		0xA1	//CMOVT.W
#define BGBCC_SH_NMID_CMOVTL		0xA2	//CMOVT.L
#define BGBCC_SH_NMID_CMOVTQ		0xA3	//CMOVT.Q
#define BGBCC_SH_NMID_CMOVFB		0xA4	//CMOVF.B
#define BGBCC_SH_NMID_CMOVFW		0xA5	//CMOVF.W
#define BGBCC_SH_NMID_CMOVFL		0xA6	//CMOVF.L
#define BGBCC_SH_NMID_CMOVFQ		0xA7	//CMOVF.Q

#define BGBCC_SH_NMID_FLDCF			0xA8	//
#define BGBCC_SH_NMID_FLDCD			0xA9	//
#define BGBCC_SH_NMID_FLDCI			0xAA	//
#define BGBCC_SH_NMID_FLDCH			0xAB	//
#define BGBCC_SH_NMID_FSTCF			0xAC	//
#define BGBCC_SH_NMID_FSTCD			0xAD	//
#define BGBCC_SH_NMID_FSTCI			0xAE	//
#define BGBCC_SH_NMID_FSTCH			0xAF	//
#define BGBCC_SH_NMID_FRCPA			0xB0	//
#define BGBCC_SH_NMID_FSQRTA		0xB1	//
#define BGBCC_SH_NMID_FRCP			0xB2	//

#define BGBCC_SH_NMID_MOVI			0xC0	//
#define BGBCC_SH_NMID_MOVIV			0xC1	//
#define BGBCC_SH_NMID_PREF			0xC2	//
#define BGBCC_SH_NMID_OCBI			0xC3	//
#define BGBCC_SH_NMID_OCBP			0xC4	//
#define BGBCC_SH_NMID_OCBWB			0xC5	//
#define BGBCC_SH_NMID_ICBI			0xC6	//
#define BGBCC_SH_NMID_MOVQ			0xC7	//MOV.Q
#define BGBCC_SH_NMID_ADDQ			0xC8	//ADD
#define BGBCC_SH_NMID_SUBQ			0xC9	//SUB
#define BGBCC_SH_NMID_MULQ			0xCA	//SUB
#define BGBCC_SH_NMID_ANDQ			0xCB	//SUB
#define BGBCC_SH_NMID_ORQ			0xCC	//SUB
#define BGBCC_SH_NMID_XORQ			0xCD	//SUB
#define BGBCC_SH_NMID_SHLLQ			0xCE	//SUB
#define BGBCC_SH_NMID_SHLRQ			0xCF	//SUB
#define BGBCC_SH_NMID_SHALQ			0xD0	//SUB
#define BGBCC_SH_NMID_SHARQ			0xD1	//SUB
#define BGBCC_SH_NMID_LDSH16		0xD2	//SUB
#define BGBCC_SH_NMID_TSTQ			0xD3	//SUB
#define BGBCC_SH_NMID_SHADQ			0xD4	//
#define BGBCC_SH_NMID_SHLDQ			0xD5	//
#define BGBCC_SH_NMID_SHLL4			0xD6	//
#define BGBCC_SH_NMID_SHLR4			0xD7	//
#define BGBCC_SH_NMID_BREQ			0xD8	//
#define BGBCC_SH_NMID_BRNE			0xD9	//
#define BGBCC_SH_NMID_BRGT			0xDA	//
#define BGBCC_SH_NMID_BRLE			0xDB	//
#define BGBCC_SH_NMID_BRGE			0xDC	//
#define BGBCC_SH_NMID_BRLT			0xDD	//
#define BGBCC_SH_NMID_ICLRMD_DQ		0xDE	//
#define BGBCC_SH_NMID_ISETMD_DQ		0xDF	//
#define BGBCC_SH_NMID_CMPQHS		0xE0	//CMPQ/HS
#define BGBCC_SH_NMID_CMPQHI		0xE1	//CMPQ/HI
#define BGBCC_SH_NMID_CMPQEQ		0xE2	//CMPQ/EQ
#define BGBCC_SH_NMID_CMPQGE		0xE3	//CMPQ/GE
#define BGBCC_SH_NMID_CMPQGT		0xE4	//CMPQ/GT
#define BGBCC_SH_NMID_CMPQPZ		0xE5	//CMPQ/PZ
#define BGBCC_SH_NMID_CMPQPL		0xE6	//CMPQ/PL
#define BGBCC_SH_NMID_NOTS			0xE7	//
#define BGBCC_SH_NMID_SHLL32		0xE8	//
#define BGBCC_SH_NMID_SHLR32		0xE9	//
#define BGBCC_SH_NMID_SHLR1			0xEA	//
#define BGBCC_SH_NMID_SHAR1			0xEB	//
#define BGBCC_SH_NMID_SHAR2			0xEC	//
#define BGBCC_SH_NMID_SHAR4			0xED	//
#define BGBCC_SH_NMID_SHAR8			0xEE	//
#define BGBCC_SH_NMID_SHAR16		0xEF	//
#define BGBCC_SH_NMID_SHAR32		0xF0	//
#define BGBCC_SH_NMID_LDSH8			0xF1	//SUB
#define BGBCC_SH_NMID_BRA8B			0xF2	//SUB
#define BGBCC_SH_NMID_SHARX			0xF3	//SUB



#define BGBCC_SH_FMID_REGREG		0x01	//Rm, Rn
#define BGBCC_SH_FMID_REGRM			0x02	//Rm
#define BGBCC_SH_FMID_REGRN			0x03	//Rn
#define BGBCC_SH_FMID_REGIMM		0x04	//Imm, Rn
#define BGBCC_SH_FMID_REGLDABS		0x05	//@(Abs), Rn
#define BGBCC_SH_FMID_REGST			0x06	//Rm, @Rn
#define BGBCC_SH_FMID_REGLD			0x07	//@Rm, Rn
#define BGBCC_SH_FMID_REGDECST		0x08	//Rm, @-Rn
#define BGBCC_SH_FMID_REGINCLD		0x09	//@Rm+, Rn
#define BGBCC_SH_FMID_REGSTR0N		0x0A	//Rm, @(Ro+Rn)
#define BGBCC_SH_FMID_REGLDR0M		0x0B	//@(Ro+Rm), Rn
#define BGBCC_SH_FMID_REGSTDISP		0x0C	//Rm, @(Rn+Disp)
#define BGBCC_SH_FMID_REGLDDISP		0x0D	//@(Rm+Disp), Rn
#define BGBCC_SH_FMID_ABS			0x0E	//@(Abs)
#define BGBCC_SH_FMID_NONE			0x0F	//-

#define BGBCC_SH_FMID_IMM			0x10	//Imm
#define BGBCC_SH_FMID_LDREG			0x11	//@Rm
#define BGBCC_SH_FMID_STREG			0x12	//@Rn
#define BGBCC_SH_FMID_IMMSTRMN		0x13	//Imm, @(Rm+Rn)
#define BGBCC_SH_FMID_REGVIMM		0x14	//<Imm>, Rn
#define BGBCC_SH_FMID_FREGREG		0x15	//FRm, FRn
#define BGBCC_SH_FMID_FREGRM		0x16	//FRm
#define BGBCC_SH_FMID_FREGRN		0x17	//FRn
#define BGBCC_SH_FMID_DREGREG		0x18	//FRm, FRn
#define BGBCC_SH_FMID_DREGRM		0x19	//FRm
#define BGBCC_SH_FMID_DREGRN		0x1A	//FRn
#define BGBCC_SH_FMID_LABEL			0x1B	//<Lbl>
#define BGBCC_SH_FMID_LBLREG		0x1C	//<Lbl>, Rn
#define BGBCC_SH_FMID_REGREGREG		0x1D	//Rn, Rm, Ro
#define BGBCC_SH_FMID_REGINCST		0x1E	//Rm, @+Rn
#define BGBCC_SH_FMID_REGDECLD		0x1F	//@Rm-, Rn

#define BGBCC_SH_FMID_REGSTRODISP	0x20	//Rm, @(Rn,Ro,Disp)
#define BGBCC_SH_FMID_REGLDRODISP	0x21	//@(Rm,Ro,Disp), Rn
#define BGBCC_SH_FMID_REGIMMREG		0x22	//Rm, Imm, Rn
#define BGBCC_SH_FMID_FREGREGREG	0x23	//FRm, FRo, FRn
#define BGBCC_SH_FMID_DREGREGREG	0x24	//DRm, DRo, DRn

#define BGBCC_SH_FMID_FREGST		0x26	//FRm, @Rn
#define BGBCC_SH_FMID_FREGLD		0x27	//@Rm, FRn
#define BGBCC_SH_FMID_FREGDECST		0x28	//FRm, @-Rn
#define BGBCC_SH_FMID_FREGINCLD		0x29	//@Rm+, FRn
#define BGBCC_SH_FMID_FREGSTR0N		0x2A	//FRm, @(Ro+Rn)
#define BGBCC_SH_FMID_FREGLDR0M		0x2B	//@(Ro+Rm), FRn
#define BGBCC_SH_FMID_FREGSTDISP	0x2C	//FRm, @(Rn+Disp)
#define BGBCC_SH_FMID_FREGLDDISP	0x2D	//@(Rm+Disp), FRn
#define BGBCC_SH_FMID_FREGSTRODISP	0x2E	//FRm, @(Rn,Ro,Disp)
#define BGBCC_SH_FMID_FREGLDRODISP	0x2F	//@(Rm,Ro,Disp), FRn

#define BGBCC_SH_FMID_REG1ABS		0x30	//Rn, @(Abs)
#define BGBCC_SH_FMID_REG2ABS		0x31	//Rm, Rn, @(Abs)

#define BGBCC_SH_FMID_DREGST		0x36	//DRm, @Rn
#define BGBCC_SH_FMID_DREGLD		0x37	//@Rm, DRn
#define BGBCC_SH_FMID_DREGDECST		0x38	//DRm, @-Rn
#define BGBCC_SH_FMID_DREGINCLD		0x39	//@Rm+, DRn
#define BGBCC_SH_FMID_DREGSTR0N		0x3A	//DRm, @(Ro+Rn)
#define BGBCC_SH_FMID_DREGLDR0M		0x3B	//@(Ro+Rm), DRn
#define BGBCC_SH_FMID_DREGSTDISP	0x3C	//DRm, @(Rn+Disp)
#define BGBCC_SH_FMID_DREGLDDISP	0x3D	//@(Rm+Disp), DRn
#define BGBCC_SH_FMID_DREGSTRODISP	0x3E	//DRm, @(Rn,Ro,Disp)
#define BGBCC_SH_FMID_DREGLDRODISP	0x3F	//@(Rm,Ro,Disp), DRn


#define BGBCC_SH_OPVTY_NONE			0x00	//None
#define BGBCC_SH_OPVTY_IMM			0x01	//#Imm
#define BGBCC_SH_OPVTY_REG			0x02	//Reg
#define BGBCC_SH_OPVTY_NAME			0x03	//Name
#define BGBCC_SH_OPVTY_DRREG		0x04	//@Reg
#define BGBCC_SH_OPVTY_DRREGI		0x05	//@Reg+
#define BGBCC_SH_OPVTY_DRREGD		0x06	//@-Reg

#define BGBCC_SH_OPVTY_RRMEM		0x08	//@(Reg,Reg)
#define BGBCC_SH_OPVTY_RDMEM		0x09	//@(Reg,Disp)

#define BGBCC_SH_RLC_REL8			0x01	//Relative BYTE (xx)
#define BGBCC_SH_RLC_REL16			0x02	//Relative WORD (xxxx)
#define BGBCC_SH_RLC_REL32			0x03	//Relative DWORD (xxxx_xxxx)
#define BGBCC_SH_RLC_ABS8			0x04	//Absolute BYTE (Low 8 bits)
#define BGBCC_SH_RLC_ABS16			0x05	//Absolute WORD (Low 16 bits)
#define BGBCC_SH_RLC_ABS32			0x06	//Absolute DWORD
#define BGBCC_SH_RLC_ABS64			0x07	//Absolute QWORD
#define BGBCC_SH_RLC_RELW12			0x08	//Relative Low 12 bits (WORD)
											//Oxxx
#define BGBCC_SH_RLC_RELW8			0x09	//Relative Low 8 bit (WORD)
											//OOxx
#define BGBCC_SH_RLC_REL32B			0x0A	//Relative DWORD (Reverse)

#define BGBCC_SH_RLC_RELW12L		0x0B	//Relative Low 12 bits (DWORD)
#define BGBCC_SH_RLC_RELW8L			0x0C	//Relative Low 8 bit (DWORD)

#define BGBCC_SH_RLC_REL20_2A		0x0D	//Relative 20 bit (BYTE, SH2A)
#define BGBCC_SH_RLC_RELW16_BJX		0x0E	//Relative 16 bit (WORD, BJX1)
											//OOxx_OOxx
#define BGBCC_SH_RLC_RELW20_BJX		0x0F	//Relative 20 bit (WORD, BJX1)
#define BGBCC_SH_RLC_REL24_BJX		0x10	//Relative 24 bit (BYTE, BJX1)
											//OOxx_xxxx
#define BGBCC_SH_RLC_REL24B_BJX		0x11	//Relative 24 bit (BYTE, BJX1, Rev)
											//OOxx_xxxx
#define BGBCC_SH_RLC_RVA32			0x12	//RVA (DWORD)
#define BGBCC_SH_RLC_RELW24_BJX		0x13	//Relative 24 bit (WORD, BJX1)
											//OOxx_xxxx

#define BGBCC_SH_RLC_RELW16C_BSR	0x18	//Relative 16 bit (BYTE, BSR1)
											//OOxx_OOxx
#define BGBCC_SH_RLC_RELW16_BSR		0x19	//Relative 16 bit (WORD, BSR1)
											//OOxx_OOxx
#define BGBCC_SH_RLC_RELW24C_BSR	0x1A	//Relative 24 bit (BYTE, BSR1)
											//OOxx_OOxx_OOxx
#define BGBCC_SH_RLC_RELW24_BSR		0x1B	//Relative 24 bit (WORD, BSR1)
											//OOxx_OOxx_OOxx

#define BGBCC_SH_RLC_ABSW16A_BSR	0x1C	//Absolute BYTE (Low 16 bits)
#define BGBCC_SH_RLC_ABSW24A_BSR	0x1D	//Absolute BYTE (Low 24 bits)
#define BGBCC_SH_RLC_ABSW32A_BSR	0x1E	//Absolute BYTE
#define BGBCC_SH_RLC_RELW8_BSR		0x1F	//Relative Low 8 bit (WORD)

#define BGBCC_SH_RLC_RELW16D_BSR	0x20	//Relative 16 bit (WORD, BSR1)
											//Oxxx_OOOx
#define BGBCC_SH_RLC_RELW24D_BSR	0x21	//Relative 24 bit (WORD, BSR1)
											//Oxxx_OOxx_OOOx
#define BGBCC_SH_RLC_RELW32D_BSR	0x22	//Relative 24 bit (WORD, BSR1)
											//Oxxx_OOxx_OOxx_OOOx

#define BGBCC_SH_RLC_RELW32C_BSR	0x23	//Relative 32 bit (BYTE, BSR1)
											//OOxx_OOxx_OOxx_OOxx

#define BGBCC_SH_RLC_ABSW16B_BSR	0x24	//Absolute BYTE
											//Oxxx_OOOx
#define BGBCC_SH_RLC_ABSW24B_BSR	0x25	//Absolute BYTE
											//Oxxx_OOxx_OOOx
#define BGBCC_SH_RLC_ABSW32B_BSR	0x26	//Absolute BYTE
											//Oxxx_OOxx_OOxx_OOOx

#define BGBCC_SH_RLC_RELW16A_BSR	0x28	//Relative 16 bit (BYTE, BSR1)
											//Oxxx_OOOx
#define BGBCC_SH_RLC_RELW24A_BSR	0x29	//Relative 24 bit (BYTE, BSR1)
											//Oxxx_OOxx_OOOx
#define BGBCC_SH_RLC_RELW32A_BSR	0x2A	//Relative 24 bit (BYTE, BSR1)
											//Oxxx_OOxx_OOxx_OOOx

// #define BGBCC_SH_RLC_ABSW48A_BSR	0x20	//Absolute BYTE
// #define BGBCC_SH_RLC_ABSW64A_BSR	0x21	//Absolute BYTE


#define BGBCC_SH_CSEG_TEXT		0x00		//.text section
#define BGBCC_SH_CSEG_STRTAB	0x01		//.strtab section
#define BGBCC_SH_CSEG_RODATA	0x02		//.rodata section
#define BGBCC_SH_CSEG_GOT		0x03		//.got section
#define BGBCC_SH_CSEG_DATA		0x04		//.data section
#define BGBCC_SH_CSEG_BSS		0x05		//.bss section
#define BGBCC_SH_CSEG_RELOC		0x06		//.reloc section

#define BGBCC_SH_CSEG_DYN		0x07		//dynamic sections

#define BGBCC_SH_CSEG_ABS		0x7F		//absolute address

#define BGBCC_SH_REGCLS_NONE	0
#define BGBCC_SH_REGCLS_GR		1	//uses a GPR
#define BGBCC_SH_REGCLS_GR2		2	//uses a pair of GPRs
#define BGBCC_SH_REGCLS_FR		3	//uses a FR
#define BGBCC_SH_REGCLS_FR2		4	//uses a pair of FRs

#define BGBCC_SH_REGCLS_VO_REF	5	//value object, pass by-ref
#define BGBCC_SH_REGCLS_VO_GR	6	//value object pass in GPR
#define BGBCC_SH_REGCLS_VO_GR2	7	//value object pass in GPR pair
#define BGBCC_SH_REGCLS_AR_REF	8	//array object, pass by-ref
// #define BGBCC_SH_REGCLS_PTR		9	//Pointer
#define BGBCC_SH_REGCLS_QGR		9	//Quad GPR
#define BGBCC_SH_REGCLS_VO_QGR	10	//value object pass in Quad GPR
#define BGBCC_SH_REGCLS_VO_QGR2	11	//value object pass in Quad GPR pair
#define BGBCC_SH_REGCLS_WGR		12	//Uses GPR, 16-bit storage

#define BGBCC_SH_REGCLS_DR		13	//uses a FR


#define BGBCC_SH_FPSCR_DN		0x00040000	//
#define BGBCC_SH_FPSCR_PR		0x00080000	//
#define BGBCC_SH_FPSCR_SZ		0x00100000	//
#define BGBCC_SH_FPSCR_FR		0x00200000	//
#define BGBCC_SH_FPSCR_VE		0x01000000	//

#define BGBCC_SH_SRFL_DQ		0x00001000	//64-bit mode (data)
#define BGBCC_SH_SRFL_JQ		0x80000000	//64-bit mode (address)

#define BGBCC_SH_MAX_CACHEVAR 5

#define BGBCC_SH_MAX_CACHEFPVAR 4


#define BGBCC_COFF_MACH_SH3			0x01A2
#define BGBCC_COFF_MACH_SH3DSP		0x01A3
#define BGBCC_COFF_MACH_SH4			0x01A6

#define BGBCC_COFF_SCNT_NOPAD		0x00000008
#define BGBCC_COFF_SCNT_CODE		0x00000020
#define BGBCC_COFF_SCNT_IDATA		0x00000040
#define BGBCC_COFF_SCNT_UDATA		0x00000080

#define BGBCC_COFF_SCNT_GPREL		0x00008000
#define BGBCC_COFF_SCNT_MEM6BIT		0x00020000

#define BGBCC_COFF_SCNT_ALGN1B		0x00100000
#define BGBCC_COFF_SCNT_ALGN2B		0x00200000
#define BGBCC_COFF_SCNT_ALGN4B		0x00300000
#define BGBCC_COFF_SCNT_ALGN8B		0x00400000
#define BGBCC_COFF_SCNT_ALGN16B		0x00500000
#define BGBCC_COFF_SCNT_ALGN32B		0x00600000
#define BGBCC_COFF_SCNT_ALGN64B		0x00700000
#define BGBCC_COFF_SCNT_ALGN128B	0x00800000
#define BGBCC_COFF_SCNT_ALGN256B	0x00900000
#define BGBCC_COFF_SCNT_ALGN512B	0x00A00000
#define BGBCC_COFF_SCNT_ALGN1KB		0x00B00000
#define BGBCC_COFF_SCNT_ALGN2KB		0x00C00000
#define BGBCC_COFF_SCNT_ALGN4KB		0x00D00000
#define BGBCC_COFF_SCNT_ALGN8KB		0x00E00000

#define BGBCC_COFF_SCNT_NRLCOVF		0x01000000
#define BGBCC_COFF_SCNT_DISCARD		0x02000000
#define BGBCC_COFF_SCNT_NOCACHE		0x04000000
#define BGBCC_COFF_SCNT_NOPAGE		0x08000000
#define BGBCC_COFF_SCNT_SHARED		0x10000000
#define BGBCC_COFF_SCNT_EXECUTE		0x20000000
#define BGBCC_COFF_SCNT_READ		0x40000000
#define BGBCC_COFF_SCNT_WRITE		0x80000000

/*
Memory Models
Default:
	Text/Data/BSS range: 16MB
	Address Space: Full 32/64

Tiny16:
	Assume all addresses fit within a 64kB space.
	Address Space: 16 bit

Small24:
	Assume Text/Data/BSS range of 64kB.
	Data/BSS range is 24 bits,
	Address Space: 24 bit

Medium24:
	Assume Text/Data/BSS range of 16MB.
	Address Space: 24 bit

Medium32:
	Assume Text/Data/BSS range of 16MB.
	Address Space: 32 bit

Large32:
	Assume Text/Data/BSS range of 4GB.
	Address Space: 32 bit.
*/

#define BGBCC_MEMMDL_DEFAULT		0	//Default
#define BGBCC_MEMMDL_TINY16			1	//64KB
#define BGBCC_MEMMDL_SMALL24		2	//64KB+16MB, 16MB
#define BGBCC_MEMMDL_MEDIUM24		3	//16MB+16MB, 16MB
#define BGBCC_MEMMDL_MEDIUM32		4	//16MB+16MB, 4GB
#define BGBCC_MEMMDL_LARGE32		4	//4GB+4GB, 4GB

#define BGBCC_SHX_GenLabelTemp(ctx)		\
	BGBCC_SHX_GenLabelTempLLn(ctx, __FILE__, __LINE__)
#define BGBCC_SHX_GenLabel(ctx)		\
	BGBCC_SHX_GenLabelLLn(ctx, __FILE__, __LINE__)

#define BGBCC_SHX_GETLREG_LO(reg)		(BGBCC_SH_REG_R0+((reg)&31))
#define BGBCC_SHX_GETLREG_HI(reg)		(BGBCC_SH_REG_R0+(((reg)&31)+1))


typedef struct BGBCC_SHX_Context_s BGBCC_SHX_Context;
typedef struct BGBCC_SHX_OpcodeArg_s BGBCC_SHX_OpcodeArg;
typedef struct BGBCC_SHX_VarSpan_s BGBCC_SHX_VarSpan;
typedef struct BGBCC_SHX_VarSpan2_s BGBCC_SHX_VarSpan2;

typedef struct BGBCC_SHX_EmitQueueOp_s BGBCC_SHX_EmitQueueOp;


struct BGBCC_SHX_Context_s {
char *sec_name[16];
byte *sec_buf[16];
byte *sec_end[16];
byte *sec_pos[16];
byte *sec_vpos[16];
u32 sec_rva[16];		//relative virtual address (image offset)
u32 sec_lva[16];		//logical virtual address
u32 sec_lsz[16];		//logical size
byte sec;
byte nsec;

byte is_le;			//is little endian
byte use_bp;		//use frame pointer
byte need_farjmp;	//function needs far32 jumps
byte need_f16jmp;	//function needs far16 jumps
byte need_n12jmp;	//function needs at least 12-bit jumps
byte is_pic;		//is PIC.
byte use_fpr;		//uses floating point registers
byte use_dbr;		//uses fp double registers
byte is_vararg;		//function is varargs
byte is_simpass;	//is simulation pass
byte is_stable;		//function is stable
byte is_addr64;		//target uses 64-bit addresses
byte is_rawasm;		//is raw assembler
byte is_leaf;		//function is a leaf function
byte is_rom;		//building a ROM image
byte is_betav;		//uses BetaVe tweaks.
byte is_mergece;	//merge CC0e/CC3e into CExx

byte no_fpu;		//no hardware FPU instructions
byte no_ext32;		//no 32-bit instruction forms
byte fpu_soft;		//use SoftFPU design.
byte fpu_lite;		//use LiteFPU design.

byte has_shad;		//has SHAD/SHLD
byte has_movi20;	//has MOVI20 and friends
byte has_misalgn;	//has misaligned load/store
byte has_bjx1mov;	//has BJX1 MOV I-forms
byte has_bjx1jmp;	//has BJX1 Branch I-forms
byte has_bjx1ari;	//has BJX1 Arithmetic I-forms
byte has_bjx1breq;	//has BJX1 BREQ/BRNE/... I-forms
byte use_onlyimm;	//use only inline immediates
byte has_bjx1egpr;	//has BJX1-64 extended GPR ops
byte has_bjx1r3mov;	//has BJX1 Reg3 ops

byte use_egpr;		//enable use of extended GPRs
byte maxreg_gpr;	//current number of GPR register-slots

byte use_memmdl;	//use memory model

byte use_emitqueue;	//use emit queue

byte emit_isprobe;	//if we are simply probing an instruction

int simfnsz;		//simulation's function size
int simfnmsz;		//simulation's min function size
int simfnnsz;		//simulation's max function size
int tr_trnum;		//trace number
int tr_opnum;		//trace op number

int fnsz_pro;		//size of prolog
int fnsz_epi;		//size of epilog
int fnsz_bod;		//size of function body
int fnsz_vtr[16];
int fnsz_lvtr[16];
int fnsz_dvtr[16];

u32 *lbl_ofs;		//label offsets
u32 *rlc_ofs;		//reloc offsets
u32 *lbl_id;		//label IDs
u32 *rlc_id;		//reloc label IDs
byte *lbl_sec;		//label section
byte *rlc_sec;		//reloc section
byte *rlc_ty;		//reloc type
s32 *lbl_chn;		//label chains
int nlbl, mlbl;
int nrlc, mrlc;
int nvlbl;
u16 lblrov;			//labels (local)
u16 lbltrov;		//labels (temp)

s32 lbl_hash[1024];		//label chain hash

char **lbln_name;	//named label names
u32 *lbln_id;		//named label IDs
int nlbln, mlbln;
int nvlbln;

u32 *lblstr_ofs;	//named label names
u32 *lblstr_id;		//named label IDs
int nlblstr, mlblstr;

// byte reg_idx[BGBCC_SH_MAX_CACHEVAR];
// byte reg_reg[BGBCC_SH_MAX_CACHEVAR];
// int reg_live;
// int reg_resv;
u32 reg_save;
// int reg_dirty;
u32 freg_save;
u32 reg_vsave;
u32 freg_vsave;

int cnt_set_fp32;
int cnt_set_fp64;

int cnt_set_dq0;
int cnt_set_dq1;

int stat_tot_dq0;
int stat_tot_dq1;
int stat_tot_dqi;

int stat_tot_imm;
int stat_tot_imm8;
int stat_tot_imm16;
int stat_tot_imm8r;
int stat_tot_imm32;

int stat_ovlbl8;

int stat_opc_tot;
int stat_opc_base16;
int stat_opc_ext8a;
int stat_opc_ext8e;
int stat_opc_extCe;
int stat_opc_extCC0;
int stat_opc_extCC3;
byte stat_opc_issfx;


int sim_voffs;		//est' offset between real PC and sim PC

s32 dfl_fpscr;		//default FPSCR state
s32 cur_fpscr;		//current FPSCR state

s32 cur_srmode;		//current SR mode
byte dfl_dq;

s64 image_base;		//image base address

int ofs_s16tab[256];
int ofs_s32tab[256];
byte idx_s16tab[256];
byte idx_s32tab[256];

s16 const_s16tab[256];
s32 const_s32tab[256];
u32 const_s16tab_rlc[256];	//reloc label type+ID
u32 const_s32tab_rlc[256];	//reloc label type+ID
byte const_ns16;
byte const_ns32;
byte nofs_s16tab;
byte nofs_s32tab;

int jitfl;

ccxl_register regalc_map[16];
byte regalc_ltcnt[16];	//lifetime count (who to evict)
byte regalc_utcnt[16];	//current use count (0=unused)
u16 regalc_save;		//register has been saved and may hold a value
u16 regalc_live;		//register is currently holding a value
u16 regalc_dirty;

ccxl_register fregalc_map[16];
byte fregalc_ltcnt[16];	//lifetime count (who to evict)
byte fregalc_utcnt[16];	//current use count (0=unused)
u16 fregalc_save;		//register has been saved and may hold a value
u16 fregalc_live;		//register is currently holding a value
u16 fregalc_dirty;

u32 sreg_live;			//scratch registers live (per 3AC operation)
u32 sreg_held;			//scratch registers live-held (multiple ops)
u32 sfreg_live;			//scratch registers live (per 3AC operation)
u32 sfreg_held;			//scratch registers live-held (multiple ops)

int iflvl_t;			//number of branches in true set
int iflvl_f;			//number of branches in false set

BGBCC_SHX_VarSpan **vspan;
int vspan_num;
int vspan_max;
int vsp_rsv;
int vsp_tcnt;

int frm_size;			//allocated size of frame
int frm_offs_lcl;		//frame offset of local vars
int frm_offs_tmp;		//frame offset of temporaries
int frm_offs_fix;		//fixed-size automatic
int lbl_ret;			//label ID for function exit
int lbl_got;			//label ID for got
int frm_offs_retstr;	//offset of return struct
int frm_offs_save;		//offset of register-save area

int lbl_rom_data_strt;
int lbl_rom_data_end;

BGBCC_SHX_Context *next;
struct BGBCC_TransState_s *tctx;
FILE *cgen_log;

char *csrept;
int cnrept;

BGBCC_SHX_EmitQueueOp *eqfree;
BGBCC_SHX_EmitQueueOp *eqstrt, *eqend;

int *got_gblidx;
int got_n_gblidx;
int got_m_gblidx;
int t_got_gblidx[4096];

int *lvt16_lbl;
s64 *lvt16_val;
int lvt16_n_idx;
int lvt16_m_idx;
int t_lvt16_lbl[1024];
s64 t_lvt16_val[1024*2];

int *genlabel_srcpos;
int genlabel_limit;
int t_genlabel_srcpos[4096];

/* Epilog Caching */
int epihash_key[1024];
int epihash_lbl[1024];

/* Prolog Caching */
int eprhash_key[1024];
int eprhash_lbl[1024];


int opcnt_hi8[256];
int opcnt_3xx[256];
int n_opcnt_3xx;

int opcnt_f0xx[256];
int opcnt_opw1;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct BGBCC_SHX_OpcodeArg_s {
byte ty;		//operand type
byte sc;		//scale for Reg/RM forms (1,2,3,4 for mem-ref)
byte ireg;		//index for Reg/RM forms
byte breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
char *name;		//name
};

struct BGBCC_SHX_VarSpan2_s {
int bbeg;				//begin (vop)
int bend;				//end (vop)
int tbeg;				//begin (vop)
int tend;				//end (vop)
int cnt;				//total times used
};

struct BGBCC_SHX_VarSpan_s {
ccxl_register reg;		//register in question
int bbeg;				//begin (vop)
int bend;				//end (vop)
int tbeg;				//begin (vop)
int tend;				//end (vop)
int cnt;				//total times used
int flag;				//span flags

BGBCC_SHX_VarSpan2 seq[64];
int nseq;
};

/*
 * Used to buffer instructions during emit.
 */
struct BGBCC_SHX_EmitQueueOp_s {
BGBCC_SHX_EmitQueueOp *next;
u16 nmid;
byte fmid;
byte rn;		//Rn
byte rm;		//Rm or Rs
byte ro;		//Ro or Rt
int imm;
int lbl;
};
