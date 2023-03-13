/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
BJX2 VM:
Will use direct linking and assume a non-modifiable program space.

*/

#define BJX2_REG_R0		0
#define BJX2_REG_R1		1
#define BJX2_REG_R2		2
#define BJX2_REG_R3		3
#define BJX2_REG_R4		4
#define BJX2_REG_R5		5
#define BJX2_REG_R6		6
#define BJX2_REG_R7		7
#define BJX2_REG_R8		8
#define BJX2_REG_R9		9
#define BJX2_REG_R10	10
#define BJX2_REG_R11	11
#define BJX2_REG_R12	12
#define BJX2_REG_R13	13
#define BJX2_REG_R14	14
#define BJX2_REG_R15	15

#define BJX2_REG_R16	16
#define BJX2_REG_R17	17
#define BJX2_REG_R18	18
#define BJX2_REG_R19	19
#define BJX2_REG_R20	20
#define BJX2_REG_R21	21
#define BJX2_REG_R22	22
#define BJX2_REG_R23	23
#define BJX2_REG_R24	24
#define BJX2_REG_R25	25
#define BJX2_REG_R26	26
#define BJX2_REG_R27	27
#define BJX2_REG_R28	28
#define BJX2_REG_R29	29
#define BJX2_REG_R30	30
#define BJX2_REG_R31	31

#define BJX2_REG_R32	32
#define BJX2_REG_R33	33
#define BJX2_REG_R34	34
#define BJX2_REG_R35	35
#define BJX2_REG_R36	36
#define BJX2_REG_R37	37
#define BJX2_REG_R38	38
#define BJX2_REG_R39	39
#define BJX2_REG_R40	40
#define BJX2_REG_R41	41
#define BJX2_REG_R42	42
#define BJX2_REG_R43	43
#define BJX2_REG_R44	44
#define BJX2_REG_R45	45
#define BJX2_REG_R46	46
#define BJX2_REG_R47	47

#define BJX2_REG_R48	48
#define BJX2_REG_R49	49
#define BJX2_REG_R50	50
#define BJX2_REG_R51	51
#define BJX2_REG_R52	52
#define BJX2_REG_R53	53
#define BJX2_REG_R54	54
#define BJX2_REG_R55	55
#define BJX2_REG_R56	56
#define BJX2_REG_R57	57
#define BJX2_REG_R58	58
#define BJX2_REG_R59	59
#define BJX2_REG_R60	60
#define BJX2_REG_R61	61
#define BJX2_REG_R62	62
#define BJX2_REG_R63	63


#define BJX2_REG_REMAP

#ifndef BJX2_REG_REMAP
#define BJX2_REG_DLR	0
#define BJX2_REG_DHR	1
#define BJX2_REG_SP		15
#endif



#if 0
#define BJX2_REG_PC		32
#define BJX2_REG_LR		33
#define BJX2_REG_SR		34
#define BJX2_REG_VBR	35
#define BJX2_REG_SPC	36
#define BJX2_REG_SSP	37
#define BJX2_REG_GBR	38
#define BJX2_REG_TBR	39
#define BJX2_REG_TTB	40
#define BJX2_REG_TEA	41
#define BJX2_REG_MMCR	42
#define BJX2_REG_EXSR	43
#define BJX2_REG_STTB	44
#define BJX2_REG_KRR	45

#define BJX2_REG_IMM	62
#define BJX2_REG_ZZR	63
#endif

#if 0
#define BJX2_REG_PC		48
#define BJX2_REG_LR		49
#define BJX2_REG_SR		50
#define BJX2_REG_VBR	51
#define BJX2_REG_SPC	52
#define BJX2_REG_SSP	53
#define BJX2_REG_GBR	54
#define BJX2_REG_TBR	55
#define BJX2_REG_TTB	56
#define BJX2_REG_TEA	57
#define BJX2_REG_MMCR	58
#define BJX2_REG_EXSR	59
#define BJX2_REG_STTB	60
#define BJX2_REG_KRR	61
#define BJX2_REG_IMM	62
#define BJX2_REG_ZZR	63
#endif

#if 1
#define BJX2_REG_PC		64
#define BJX2_REG_LR		65
#define BJX2_REG_SR		66
#define BJX2_REG_VBR	67
#define BJX2_REG_SPC	68
#define BJX2_REG_SSP	69
#define BJX2_REG_GBR	70
#define BJX2_REG_TBR	71
#define BJX2_REG_TTB	72
#define BJX2_REG_TEA	73
#define BJX2_REG_MMCR	74
#define BJX2_REG_EXSR	75
#define BJX2_REG_STTB	76
#define BJX2_REG_KRR	77
// #define BJX2_REG_IMM	78
// #define BJX2_REG_ZZR	79

#define BJX2_REG_ZZR	95

#endif

#define BJX2_REG_PC2	80
#define BJX2_REG_LR2	81
#define BJX2_REG_SR2	82
#define BJX2_REG_TEAH	83
#define BJX2_REG_SPC2	84
#define BJX2_REG_SSP2	85
#define BJX2_REG_GBR2	86
#define BJX2_REG_TBR2	87

#ifdef BJX2_REG_REMAP

#define BJX2_REG_R15A	79

// #define BJX2_REG_DLR	88
// #define BJX2_REG_DHR	89
// #define BJX2_REG_SP		90

#define BJX2_REG_DLR	96
#define BJX2_REG_DHR	97
#define BJX2_REG_SP		(96+15)

#endif

#define BJX2_REG_PC_HI	98
#define BJX2_REG_VBR_HI	99
#define BJX2_REG_SPC_HI	100
//#define BJX2_REG_SSP_HI	101
#define BJX2_REG_GBR_HI	102
// #define BJX2_REG_SP_HI	103
// #define BJX2_REG_PC_HI	104
// #define BJX2_REG_LR_HI	105


#define BJX2_REG_DR		BJX2_REG_DLR



#if 0
#define BJX2_REG_R0B	64
#define BJX2_REG_R1B	65
#define BJX2_REG_R2B	66
#define BJX2_REG_R3B	67
#define BJX2_REG_R4B	68
#define BJX2_REG_R5B	69
#define BJX2_REG_R6B	70
#define BJX2_REG_R7B	71
// #define BJX2_REG_SPC	72
#define BJX2_REG_SLR	73
#define BJX2_REG_SSR	74
// #define BJX2_REG_SSP	75
#define BJX2_REG_SDL	76
#define BJX2_REG_SDH	77
#define BJX2_REG_SGB	78
#define BJX2_REG_STB	79
#define BJX2_REG_R16B	80
#define BJX2_REG_R17B	81
#define BJX2_REG_R18B	82
#define BJX2_REG_R19B	83
#define BJX2_REG_R20B	84
#define BJX2_REG_R21B	85
#define BJX2_REG_R22B	86
#define BJX2_REG_R23B	87
#endif

// #define BJX2_TR_MAXOP	32
// #define BJX2_TR_MAXOP	40
#define BJX2_TR_MAXOP	64


#define BJX2_FLT_INVADDR	0x8000		//invalid memory address
#define BJX2_FLT_INV_MRD	0x8001		//invalid memory read
#define BJX2_FLT_INV_MWR	0x8002		//invalid memory write
#define BJX2_FLT_INV_MEX	0x8003		//invalid memory execute
#define BJX2_FLT_BREAK		0x8004		//BREAK
#define BJX2_FLT_SLEEP		0x8005		//SLEEP
#define BJX2_FLT_TRAP		0x8006		//Invalid Misaligned Access
#define BJX2_FLT_PAGEFLT	0x8007		//Invalid Misaligned Access

#define BJX2_FLT_MISAL		0x8008		//Invalid Misaligned Access
#define BJX2_FLT_MISAL_R	0x8009		//Invalid Misaligned Access (R)
#define BJX2_FLT_MISAL_W	0x800A		//Invalid Misaligned Access (W)
#define BJX2_FLT_MISAL_X	0x800B		//Invalid Misaligned Access (X)
#define BJX2_FLT_INVSMT		0x800C		//Invalid SMT
#define BJX2_FLT_INVKRR		0x800D		//Invalid KRR
#define BJX2_FLT_INVOP		0x800E		//Invalid Opcode
#define BJX2_FLT_BOUNDCHK	0x800F		//Bounds Check Fail

#define BJX2_FLT_PCMISH		0x8801		//PC doesn't match trace addr
#define BJX2_FLT_CCFLUSH	0x8802		//Cache Flush
#define BJX2_FLT_BADPC		0x8803		//Invalid PC

#define BJX2_FLT_TLBGF		0xA000		//TLB General Fault
#define BJX2_FLT_TLBMISS	0xA001		//TLB Miss
#define BJX2_FLT_TLBACL		0xA002		//TLB ACL Check

#define BJX2_FLT_TIMER		0xC001		//kHz timer
#define BJX2_FLT_IOPOKE		0xC002		//IO Poke
#define BJX2_FLT_SCRPOKE	0xC003		//Screen Poke
#define BJX2_FLT_EMUBREAK	0xC004		//Emulator Breakpoint

#define BJX2_OPFL_CTRLF		0x0001		//Control-Flow Opcode
#define BJX2_OPFL_TWOWORD	0x0002		//Uses two instruction words
#define BJX2_OPFL_TRIWORD	0x0004		//Uses three instruction words
#define BJX2_OPFL_WEX		0x0008		//Wide-Executed Op
#define BJX2_OPFL_JUMBO64	0x0010		//Uses four instruction words
#define BJX2_OPFL_JUMBO96	0x0020		//Uses six instruction words
#define BJX2_OPFL_NOWEX		0x0040		//Invalid as a WEX form
#define BJX2_OPFL_NOWEXSFX	0x0080		//Invalid as a WEX suffix

#define BJX2_OPFL_REGXM		0x0100		//Rm is Xm
#define BJX2_OPFL_REGXN		0x0200		//Rn is Xn
#define BJX2_OPFL_REGXO		0x0400		//Ro is Xo
#define BJX2_OPFL_REGX2R	0x0300		//Rm and Rn are Xn
#define BJX2_OPFL_REGX3R	0x0700		//Rm, Rn, and Ro are Xn
// #define BJX2_OPFL_OP24		0x0800		//24-bit instruction
#define BJX2_OPFL_OPPIPE	0x0800		//Pipelined Opcode
#define BJX2_OPFL_OPSSC		0x1000		//Op Flagged as Superscalar

#define BJX2_OPFL_NOWEX_FP2		0x2000		//Not valid WEX unless FP2
#define BJX2_OPFL_NOWEX_IO2		0x4000		//Not valid WEX unless IO2
#define BJX2_OPFL_NOWEXSFX3W	0x8000		//Not valid WEX suffix if 3-Wide

#define BJX2_OPFL_PREDMSK	0xC1		//Mask copied for predicated ops.


#define BJX2_NMID_NONE		0x00		//
#define BJX2_NMID_MOVB		0x01		//
#define BJX2_NMID_MOVW		0x02		//
#define BJX2_NMID_MOVL		0x03		//
#define BJX2_NMID_MOVQ		0x04		//
#define BJX2_NMID_MOVUB		0x05		//
#define BJX2_NMID_MOVUW		0x06		//
#define BJX2_NMID_NOP		0x07		//
#define BJX2_NMID_BRA		0x08		//
#define BJX2_NMID_BSR		0x09		//
#define BJX2_NMID_BT		0x0A		//
#define BJX2_NMID_BF		0x0B		//
#define BJX2_NMID_LEAB		0x0C		//
#define BJX2_NMID_LEAW		0x0D		//
#define BJX2_NMID_LEAD		0x0E		//
#define BJX2_NMID_LEAQ		0x0F		//

#define BJX2_NMID_ADD		0x10		//
#define BJX2_NMID_SUB		0x11		//
#define BJX2_NMID_ADC		0x12		//
#define BJX2_NMID_SBB		0x13		//
#define BJX2_NMID_TST		0x14		//
#define BJX2_NMID_AND		0x15		//
#define BJX2_NMID_OR		0x16		//
#define BJX2_NMID_XOR		0x17		//
#define BJX2_NMID_MOV		0x18		//
#define BJX2_NMID_LDIZ		0x19		//
#define BJX2_NMID_LDIN		0x1A		//
#define BJX2_NMID_LDISH8	0x1B		//
#define BJX2_NMID_CMPEQ		0x1C		//
#define BJX2_NMID_CMPHI		0x1D		//
#define BJX2_NMID_CMPGT		0x1E		//
#define BJX2_NMID_RTS		0x1F		//

#define BJX2_NMID_EXTUB		0x20		//
#define BJX2_NMID_EXTUW		0x21		//
#define BJX2_NMID_EXTSB		0x22		//
#define BJX2_NMID_EXTSW		0x23		//
#define BJX2_NMID_CLRT		0x24		//
#define BJX2_NMID_SETT		0x25		//
#define BJX2_NMID_CLRS		0x26		//
#define BJX2_NMID_SETS		0x27		//
#define BJX2_NMID_NOTT		0x28		//
#define BJX2_NMID_NOTS		0x29		//
#define BJX2_NMID_LDISH16	0x2A		//
#define BJX2_NMID_CMPGE		0x2B		//
#define BJX2_NMID_PUSH		0x2C		//
#define BJX2_NMID_POP		0x2D		//
#define BJX2_NMID_CMPPL		0x2E		//
#define BJX2_NMID_CMPPZ		0x2F		//

#define BJX2_NMID_SHLL		0x30		//
#define BJX2_NMID_SHLR		0x31		//
#define BJX2_NMID_SHAR		0x32		//
#define BJX2_NMID_CMPHS		0x33		//
#define BJX2_NMID_BREAK		0x34		//
#define BJX2_NMID_SLEEP		0x35		//
#define BJX2_NMID_MULS		0x36		//
#define BJX2_NMID_MULU		0x37		//
#define BJX2_NMID_RTE		0x38		//
#define BJX2_NMID_NOT		0x39		//
#define BJX2_NMID_NEG		0x3A		//
#define BJX2_NMID_NEGC		0x3B		//
#define BJX2_NMID_ROTL		0x3C		//
#define BJX2_NMID_ROTR		0x3D		//
#define BJX2_NMID_ROTCL		0x3E		//
#define BJX2_NMID_ROTCR		0x3F		//

#define BJX2_NMID_RET		0x40		//
#define BJX2_NMID_MOVT		0x41		//
#define BJX2_NMID_LDISH12	0x42		//
#define BJX2_NMID_LDISH20	0x43		//
#define BJX2_NMID_SHAD		0x44		//
#define BJX2_NMID_SHLD		0x45		//
#define BJX2_NMID_TRAP		0x46		//
#define BJX2_NMID_WEXMD		0x47		//
#define BJX2_NMID_FADD		0x48		//
#define BJX2_NMID_FSUB		0x49		//
#define BJX2_NMID_FMUL		0x4A		//
#define BJX2_NMID_FDIV		0x4B		//
#define BJX2_NMID_FCMPEQ	0x4C		//
#define BJX2_NMID_FCMPGT	0x4D		//
#define BJX2_NMID_FMOVS		0x4E		//
#define BJX2_NMID_FMOVH		0x4F		//
// #define BJX2_NMID_FMOVD		0x4F		//

#define BJX2_NMID_FMOV		0x50		//
#define BJX2_NMID_FNEG		0x51		//
#define BJX2_NMID_FLDCF		0x52		//
#define BJX2_NMID_FSTCF		0x53		//
#define BJX2_NMID_FLDCI		0x54		//
#define BJX2_NMID_FSTCI		0x55		//
#define BJX2_NMID_FLDCH		0x56		//
#define BJX2_NMID_FSTCH		0x57		//
#define BJX2_NMID_FLDCD		0x58		//
#define BJX2_NMID_FSTCD		0x59		//
#define BJX2_NMID_FABS		0x5A		//
#define BJX2_NMID_INV		0x5B		//
#define BJX2_NMID_MOVUL		0x5C		//
#define BJX2_NMID_EXTUL		0x5D		//
#define BJX2_NMID_EXTSL		0x5E		//
#define BJX2_NMID_LDISH32	0x5F		//

#define BJX2_NMID_SHADQ		0x60		//
#define BJX2_NMID_SHLDQ		0x61		//
#define BJX2_NMID_TSTQ		0x62		//
#define BJX2_NMID_CMPQEQ	0x63		//
#define BJX2_NMID_CMPQGT	0x64		//
#define BJX2_NMID_CMPQHI	0x65		//
#define BJX2_NMID_CMPQGE	0x66		//
#define BJX2_NMID_CMPQHS	0x67		//
#define BJX2_NMID_FPUSH		0x68		//
#define BJX2_NMID_FPOP		0x69		//
#define BJX2_NMID_CSELT		0x6A		//
#define BJX2_NMID_MOVNT		0x6B		//
#define BJX2_NMID_CLZ		0x6C		//
#define BJX2_NMID_CLZQ		0x6D		//
#define BJX2_NMID_MOVDL		0x6E		//
#define BJX2_NMID_LDI		0x6F		//

#define BJX2_NMID_SWAPB		0x70		//
#define BJX2_NMID_SWAPW		0x71		//
#define BJX2_NMID_SWAP8B	0x72		//
#define BJX2_NMID_SWAPL		0x73		//
#define BJX2_NMID_SWAPLB	0x74		//
#define BJX2_NMID_SWAPLW	0x75		//
#define BJX2_NMID_SWCPLB	0x76		//
#define BJX2_NMID_SWCPLW	0x77		//
#define BJX2_NMID_SWAPMB	0x78		//
#define BJX2_NMID_SWAPMW	0x79		//
#define BJX2_NMID_SWCPMB	0x7A		//
#define BJX2_NMID_SWCPMW	0x7B		//
#define BJX2_NMID_SWAPHB	0x7C		//
#define BJX2_NMID_SWAPHW	0x7D		//
#define BJX2_NMID_SWCPHB	0x7E		//
#define BJX2_NMID_SWCPHW	0x7F		//

#define BJX2_NMID_MOVHD		0x80		//
#define BJX2_NMID_MOVLD		0x81		//
#define BJX2_NMID_MOVHLD	0x82		//
#define BJX2_NMID_MOVLHD	0x83		//
#define BJX2_NMID_MOVD		0x84		//
#define BJX2_NMID_FMOVUS	0x85		//
#define BJX2_NMID_PRED_T	0x86		//
#define BJX2_NMID_PRED_F	0x87		//
#define BJX2_NMID_ADDSL		0x88		//
#define BJX2_NMID_ADDUL		0x89		//
#define BJX2_NMID_SUBSL		0x8A		//
#define BJX2_NMID_SUBUL		0x8B		//
#define BJX2_NMID_PADDW		0x8C		//
#define BJX2_NMID_PADDL		0x8D		//
#define BJX2_NMID_PSUBW		0x8E		//
#define BJX2_NMID_PSUBL		0x8F		//

#define BJX2_NMID_PMULUW	0x90		//
#define BJX2_NMID_PADDH		0x91		//
#define BJX2_NMID_PSUBH		0x92		//
#define BJX2_NMID_PMULH		0x93		//
#define BJX2_NMID_INVDC		0x94		//
#define BJX2_NMID_INVIC		0x95		//
#define BJX2_NMID_INVTLB	0x96		//
#define BJX2_NMID_RTSU		0x97		//
#define BJX2_NMID_MULSW		0x98		//
#define BJX2_NMID_MULUW		0x99		//
#define BJX2_NMID_SYSCALL	0x9A		//
#define BJX2_NMID_LDIQ		0x9B		//
#define BJX2_NMID_JLDI		0x9C		//
#define BJX2_NMID_MOVX2		0x9D		//
#define BJX2_NMID_PUSHX2	0x9E		//
#define BJX2_NMID_POPX2		0x9F		//

#define BJX2_NMID_MULSL		0xA0		//
#define BJX2_NMID_MULUL		0xA1		//
#define BJX2_NMID_LDIL		0xA2		//
#define BJX2_NMID_CPUID		0xA3		//
#define BJX2_NMID_FLDCFH	0xA4		//
#define BJX2_NMID_FSTCFH	0xA5		//
#define BJX2_NMID_PSHUFB	0xA6		//
#define BJX2_NMID_RGBSHR1	0xA7		//
#define BJX2_NMID_RGBAVG	0xA8		//
#define BJX2_NMID_RGBMOD	0xA9		//
#define BJX2_NMID_RGBPCK	0xAA		//
#define BJX2_NMID_PSHUFW	0xAB		//
#define BJX2_NMID_PMULSLW	0xAC		//
#define BJX2_NMID_PMULULW	0xAD		//
#define BJX2_NMID_PMULSHW	0xAE		//
#define BJX2_NMID_PMULUHW	0xAF		//

#define BJX2_NMID_PADDF			0xB0		//
#define BJX2_NMID_PADDXF		0xB1		//
#define BJX2_NMID_PSUBF			0xB2		//
#define BJX2_NMID_PSUBXF		0xB3		//
#define BJX2_NMID_PMULF			0xB4		//
#define BJX2_NMID_PMULXF		0xB5		//
#define BJX2_NMID_CTZ			0xB6		//
#define BJX2_NMID_CTZQ			0xB7		//
#define BJX2_NMID_BTRNS			0xB8		//
#define BJX2_NMID_BTRNSQ		0xB9		//
#define BJX2_NMID_PMORTL		0xBA		//
#define BJX2_NMID_PMORTQ		0xBB		//
#define BJX2_NMID_SHLRQ			0xBC		//
#define BJX2_NMID_SHARQ			0xBD		//
#define BJX2_NMID_RGB32PCK64	0xBE		//
#define BJX2_NMID_RGB32UPCK64	0xBF		//

#define BJX2_NMID_RGB5PCK32		0xC0		//
#define BJX2_NMID_RGB5UPCK32	0xC1		//
#define BJX2_NMID_RGB5PCK64		0xC2		//
#define BJX2_NMID_RGB5UPCK64	0xC3		//
#define BJX2_NMID_LDTLB			0xC4		//
#define BJX2_NMID_PADDXD		0xC5		//
#define BJX2_NMID_PSUBXD		0xC6		//
#define BJX2_NMID_PMULXD		0xC7		//
#define BJX2_NMID_PCSELTW		0xC8		//
#define BJX2_NMID_PCSELTL		0xC9		//
#define BJX2_NMID_PCMPEQH		0xCA		//
#define BJX2_NMID_PCMPGTH		0xCB		//
#define BJX2_NMID_PCMPEQF		0xCC		//
#define BJX2_NMID_PCMPGTF		0xCD		//
#define BJX2_NMID_PCMPEQW		0xCE		//
#define BJX2_NMID_PCMPEQL		0xCF		//

#define BJX2_NMID_PCMPHIW		0xD0		//
#define BJX2_NMID_PCMPGTW		0xD1		//
#define BJX2_NMID_PCMPHIL		0xD2		//
#define BJX2_NMID_PCMPGTL		0xD3		//
#define BJX2_NMID_FADDX			0xD4		//
#define BJX2_NMID_FSUBX			0xD5		//
#define BJX2_NMID_FMULX			0xD6		//
#define BJX2_NMID_PCONV			0xD7		//
#define BJX2_NMID_FCMPXEQ		0xD8		//
#define BJX2_NMID_FCMPXGT		0xD9		//
#define BJX2_NMID_PSCHEQW		0xDA		//
#define BJX2_NMID_PSCHNEW		0xDB		//
#define BJX2_NMID_JADD			0xDC		//
#define BJX2_NMID_CMPXEQ		0xDD		//
#define BJX2_NMID_CMPXGT		0xDE		//
#define BJX2_NMID_CMPXHI		0xDF		//

#define BJX2_NMID_ADDX			0xE0		//
#define BJX2_NMID_SUBX			0xE1		//
#define BJX2_NMID_ADCX			0xE2		//
#define BJX2_NMID_SBBX			0xE3		//
#define BJX2_NMID_ROTLX			0xE4		//
#define BJX2_NMID_ANDX			0xE5		//
#define BJX2_NMID_ORX			0xE6		//
#define BJX2_NMID_XORX			0xE7		//
#define BJX2_NMID_SHADX			0xE8		//
#define BJX2_NMID_SHLDX			0xE9		//
#define BJX2_NMID_SHARX			0xEA		//
#define BJX2_NMID_SHLRX			0xEB		//
#define BJX2_NMID_BLKUTX1		0xEC		//
#define BJX2_NMID_BLKUTX2		0xED		//
#define BJX2_NMID_BLKUTX3H		0xEE		//
#define BJX2_NMID_BLKUTX3L		0xEF		//

#define BJX2_NMID_BLINT			0xF0		//
#define BJX2_NMID_BLINTA		0xF1		//
#define BJX2_NMID_BLERP			0xF2		//
#define BJX2_NMID_LDACL			0xF3		//
#define BJX2_NMID_PLDCH			0xF4		//
#define BJX2_NMID_PLDCHH		0xF5		//
#define BJX2_NMID_PSTCH			0xF6		//
#define BJX2_NMID_PSTCHH		0xF7		//
#define BJX2_NMID_PLDCXH		0xF8		//
#define BJX2_NMID_PSTCXH		0xF9		//
#define BJX2_NMID_PLDCM30AH		0xFA		//
#define BJX2_NMID_PSTCM30AH		0xFB		//
#define BJX2_NMID_PLDCM8SH		0xFC		//
#define BJX2_NMID_PLDCM8UH		0xFD		//
#define BJX2_NMID_PSTCM8SH		0xFE		//
#define BJX2_NMID_PSTCM8UH		0xFF		//

#define BJX2_NMID_PSCHEQB		0x100		//
#define BJX2_NMID_PSCHNEB		0x101		//
#define BJX2_NMID_BLKUAB1		0x102		//
#define BJX2_NMID_BLKUAB2		0x103		//
#define BJX2_NMID_CONVFXI		0x104		//
#define BJX2_NMID_CONVFLI		0x105		//
#define BJX2_NMID_SNIPEDC		0x106		//
#define BJX2_NMID_SNIPEIC		0x107		//
#define BJX2_NMID_SXENTR		0x108		//
#define BJX2_NMID_SUENTR		0x109		//
#define BJX2_NMID_SVEKRR		0x10A		//
#define BJX2_NMID_SVENTR		0x10B		//
#define BJX2_NMID_LDEKRR		0x10C		//
#define BJX2_NMID_LDEKEY		0x10D		//
#define BJX2_NMID_LDEENC		0x10E		//
#define BJX2_NMID_FDIVX			0x10F		//

#define BJX2_NMID_BREQ			0x110		//
#define BJX2_NMID_BRNE			0x111		//
#define BJX2_NMID_BRLT			0x112		//
#define BJX2_NMID_BRGE			0x113		//
#define BJX2_NMID_BRBI			0x114		//
#define BJX2_NMID_BRHE			0x115		//
#define BJX2_NMID_BRTSTT		0x116		//
#define BJX2_NMID_BRTSTF		0x117		//
#define BJX2_NMID_BRAL			0x118		//
#define BJX2_NMID_BSRL			0x119		//
#define BJX2_NMID_BTL			0x11A		//
#define BJX2_NMID_BFL			0x11B		//
#define BJX2_NMID_CMPTAEQ		0x11C		//
#define BJX2_NMID_CMPTAHI		0x11D		//
#define BJX2_NMID_CMPTAHS		0x11E		//
#define BJX2_NMID_CMPTTEQ		0x11F		//

#define BJX2_NMID_MACSL			0x120		//
#define BJX2_NMID_MACUL			0x121		//
#define BJX2_NMID_DMACSL		0x122		//
#define BJX2_NMID_DMACUL		0x123		//
#define BJX2_NMID_XLEAB			0x124		//
#define BJX2_NMID_XLEAW			0x125		//
#define BJX2_NMID_XLEAL			0x126		//
#define BJX2_NMID_XLEAQ			0x127		//
#define BJX2_NMID_BRAX			0x128		//
#define BJX2_NMID_BSRX			0x129		//
#define BJX2_NMID_BTX			0x12A		//
#define BJX2_NMID_BFX			0x12B		//
#define BJX2_NMID_MOVTT			0x12C		//
#define BJX2_NMID_XMOVTT		0x12D		//
#define BJX2_NMID_FSQRT			0x12E		//
#define BJX2_NMID_FSQRTX		0x12F		//

#define BJX2_NMID_MULSQ			0x130		//
#define BJX2_NMID_MULUQ			0x131		//
#define BJX2_NMID_MULSHQ		0x132		//
#define BJX2_NMID_MULUHQ		0x133		//
#define BJX2_NMID_DIVSQ			0x134		//
#define BJX2_NMID_DIVUQ			0x135		//
#define BJX2_NMID_MODSQ			0x136		//
#define BJX2_NMID_MODUQ			0x137		//
#define BJX2_NMID_FDIVA			0x138		//
#define BJX2_NMID_FDIVXA		0x139		//
#define BJX2_NMID_FSQRTA		0x13A		//
#define BJX2_NMID_FSQRTXA		0x13B		//
#define BJX2_NMID_BCDADC		0x13C		//
#define BJX2_NMID_BCDSBB		0x13D		//
#define BJX2_NMID_ROTCLQ		0x13E		//
#define BJX2_NMID_ROTCRQ		0x13F		//

#define BJX2_NMID_LDTEX			0x140		//
#define BJX2_NMID_RSUB			0x141		//
#define BJX2_NMID_JLDIT			0x142		//
#define BJX2_NMID_JLDIF			0x143		//
#define BJX2_NMID_DIVSL			0x144		//
#define BJX2_NMID_DIVUL			0x145		//
#define BJX2_NMID_MODSL			0x146		//
#define BJX2_NMID_MODUL			0x147		//
#define BJX2_NMID_CSRRW			0x148		//(CSR SWAP, RISC-V)
#define BJX2_NMID_CSRRS			0x149		//(CSR SET, RISC-V)
#define BJX2_NMID_CSRRC			0x14A		//(CSR CLEAR, RISC-V)
#define BJX2_NMID_CSRRWI		0x14B		//(CSR SWAP, RISC-V)
#define BJX2_NMID_CSRRSI		0x14C		//(CSR SET, RISC-V)
#define BJX2_NMID_CSRRCI		0x14D		//(CSR CLEAR, RISC-V)
#define BJX2_NMID_VSKG			0x14E		//
#define BJX2_NMID_VSKC			0x14F		//

#define BJX2_NMID_LDOPB			0x150		//
#define BJX2_NMID_LDOPUB		0x151		//
#define BJX2_NMID_LDOPW			0x152		//
#define BJX2_NMID_LDOPUW		0x153		//
#define BJX2_NMID_LDOPL			0x154		//
#define BJX2_NMID_LDOPUL		0x155		//
#define BJX2_NMID_LDOPQ			0x156		//
#define BJX2_NMID_LDOPUQ		0x157		//
#define BJX2_NMID_BREQL			0x158		//
#define BJX2_NMID_BRNEL			0x159		//
#define BJX2_NMID_BRLTL			0x15A		//
#define BJX2_NMID_BRGEL			0x15B		//
#define BJX2_NMID_BRGTL			0x15C		//
#define BJX2_NMID_BRLEL			0x15D		//
#define BJX2_NMID_BRLE			0x15E		//
#define BJX2_NMID_BRGT			0x15F		//

#define BJX2_NMID_PCVTSB2HL		0x160		//
#define BJX2_NMID_PCVTUB2HL		0x161		//
#define BJX2_NMID_PCVTSB2HH		0x162		//
#define BJX2_NMID_PCVTUB2HH		0x163		//
#define BJX2_NMID_PCVTSW2FL		0x164		//
#define BJX2_NMID_PCVTUW2FL		0x165		//
#define BJX2_NMID_PCVTSW2FH		0x166		//
#define BJX2_NMID_PCVTUW2FH		0x167		//
#define BJX2_NMID_PCVTH2SB		0x168		//
#define BJX2_NMID_PCVTH2UB		0x169		//
#define BJX2_NMID_PCVTSW2H		0x16A		//
#define BJX2_NMID_PCVTUW2H		0x16B		//
#define BJX2_NMID_PCVTF2SW		0x16C		//
#define BJX2_NMID_PCVTF2UW		0x16D		//
#define BJX2_NMID_PCVTH2SW		0x16E		//
#define BJX2_NMID_PCVTH2UW		0x16F		//

#define BJX2_NMID_PSQRTAH		0x170	//
#define BJX2_NMID_PSQRTUAH		0x171	//
#define BJX2_NMID_PSQRTAF		0x172	//
#define BJX2_NMID_PSQRTUAF		0x173	//
#define BJX2_NMID_PRCPAH		0x174	//
#define BJX2_NMID_PRELUH		0x175	//
#define BJX2_NMID_PRCPAF		0x176	//
#define BJX2_NMID_PRELUF		0x177	//
#define BJX2_NMID_BNDCMP		0x178	//
#define BJX2_NMID_FCMPGE		0x179		//
#define BJX2_NMID_REGCHKG		0x17A		//
#define BJX2_NMID_REGCHKC		0x17B		//
#define BJX2_NMID_EMUBREAK		0x17C		//

// #define BJX2_NMID_PLDCXH		0x17A		//
// #define BJX2_NMID_PSTCXH		0x17B		//

#define BJX2_NMID_BNDCHKB		0x180	//
#define BJX2_NMID_BNDCHKW		0x181	//
#define BJX2_NMID_BNDCHKL		0x182	//
#define BJX2_NMID_BNDCHKQ		0x183	//
#define BJX2_NMID_LEATB			0x184	//
#define BJX2_NMID_LEATW			0x185	//
#define BJX2_NMID_LEATL			0x186	//
#define BJX2_NMID_LEATQ			0x187	//

#define BJX2_NMID_BNDCMPB		0x188	//
#define BJX2_NMID_BNDCMPW		0x189	//
#define BJX2_NMID_BNDCMPL		0x18A	//
#define BJX2_NMID_BNDCMPQ		0x18B	//

#define BJX2_NMID_MOVZT			0x18C		//
#define BJX2_NMID_XMOVZT		0x18D		//
#define BJX2_NMID_MOVST			0x18E		//
#define BJX2_NMID_XMOVST		0x18F		//



#define BJX2_FMID_NONE			0x00		//?
#define BJX2_FMID_REG			0x01		//Rn
#define BJX2_FMID_REGREG		0x02		//Rm, Rn
#define BJX2_FMID_Z				0x03		//-
#define BJX2_FMID_IMM			0x04		//#imm
#define BJX2_FMID_PCDISP		0x05		//(PC, disp)
#define BJX2_FMID_LDREGREG		0x06		//(Rm), Rn
#define BJX2_FMID_REGSTREG		0x07		//Rm, (Rn)
#define BJX2_FMID_LDDRREGREG	0x08		//(Rm, DLR), Rn
#define BJX2_FMID_REGSTDRREG	0x09		//Rm, (Rn, DLR)
#define BJX2_FMID_LDREGDISPREG	0x0A		//(Rm, Disp), Rn
#define BJX2_FMID_REGSTREGDISP	0x0B		//Rm, (Rn, Disp)
#define BJX2_FMID_PCDR			0x0C		//(PC, DLR)
#define BJX2_FMID_LDDRPCREG		0x0D		//(PC, DLR), Rn
#define BJX2_FMID_REGSTDRPC		0x0E		//Rm, (PC, DLR)
#define BJX2_FMID_IMMREG		0x0F		//#imm, Rn
#define BJX2_FMID_LDDRABSREG	0x10		//(DLR), Rn
#define BJX2_FMID_REGSTDRABS	0x11		//Rm, (DLR)
#define BJX2_FMID_PCDR4			0x12		//(PC, DLR_i4)
#define BJX2_FMID_LDDR4PCREG	0x13		//(PC, DLR_i4), Rn
#define BJX2_FMID_REGSTDR4PC	0x14		//Rm, (PC, DLR_i4)
#define BJX2_FMID_DR4REG		0x15		//DLR_i4, Rn
#define BJX2_FMID_LDPCDISPREG	0x16		//(PC, Disp), Rn
#define BJX2_FMID_REGSTPCDISP	0x17		//Rm, (PC, Disp)
#define BJX2_FMID_REGDRREG		0x18		//Rm, DLR, Rn
#define BJX2_FMID_PCREG			0x19		//(PC, Reg)
#define BJX2_FMID_LDREG2REG		0x1A		//(Rm, Ro), Rn
#define BJX2_FMID_REGSTREG2		0x1B		//Rm, (Rn, Ro)
#define BJX2_FMID_REGREGREG		0x1C		//Rm, Ro, Rn
#define BJX2_FMID_REGIMMREG		0x1D		//Rm, Imm, Rn
// #define BJX2_FMID_IMMXREG		0x1E		//#immx, Rn

#define BJX2_FMID_IMMZREG		0x1E		//#immu, Rn
#define BJX2_FMID_IMMNREG		0x1F		//#immn, Rn

#define BJX2_FMID_FREG			0x20		//FRn
#define BJX2_FMID_FREGREG		0x21		//FRm, FRn
#define BJX2_FMID_FREGREGREG	0x22		//FRm, FRo, FRn

#define BJX2_FMID_IMMJREG		0x23		//#jimm, Rn

#define BJX2_FMID_FREGGREG		0x24		//FRm, Rn
#define BJX2_FMID_GREGFREG		0x25		//Rm, FRn

#define BJX2_FMID_CHAIN			0x27		//Chained Opcode

#define BJX2_FMID_REGPCDISP		0x28		//Reg, (PC, disp)
#define BJX2_FMID_REGREGPCDISP	0x29		//Reg, Reg, (PC, disp)
#define BJX2_FMID_LDREGDISP1REG	0x2A		//(Rm, Disp1), Rn
#define BJX2_FMID_REGSTREGDISP1	0x2B		//Rm, (Rn, Disp1)
#define BJX2_FMID_LDREGDISP1	0x2C		//(Rn, Disp1)

#define BJX2_FMID_REGREGREGREG	0x2E		//Rm, Ro, Rp, Rn
#define BJX2_FMID_REGIMMREGREG	0x2F		//Rm, Imm, Rp, Rn

#define BJX2_FLAG_SR_T			0x00000001ULL
#define BJX2_FLAG_SR_S			0x00000002ULL

#define BJX2_FLAG_SR_XG2		0x00800000ULL
#define BJX2_FLAG_SR_RVE		0x04000000ULL
#define BJX2_FLAG_SR_WXE		0x08000000ULL
#define BJX2_FLAG_SR_BL			0x10000000ULL
#define BJX2_FLAG_SR_RB			0x20000000ULL
#define BJX2_FLAG_SR_MD			0x40000000ULL
#define BJX2_FLAG_SR_JQ			0x80000000ULL

#define BJX2_CFG_SIMDFXASS

#define BJX2_FLIPSTNM

// #ifdef X86_64
#if defined(X86_64) && !defined(BJX2_EM_NOSTAT)

#define BJX2_EM_BPRED
#define BJX2_EM_MEMSTAT

#endif


#if 0
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#ifndef NLINT_T
#define NLINT_T
typedef signed long long nlint;
#endif

#endif

#if defined(ARM)
// #if 1
// #define BJX2_ADDR32
#endif

#ifdef BJX2_ADDR32
typedef u32 bjx2_addr;
typedef u32 bjx2_addru;
#else
typedef s64 bjx2_addr;
typedef u64 bjx2_addru;
#endif

typedef unsigned char jx2_bool;

#ifdef _MSC_VER
#ifndef default_inline
#define default_inline __inline
#define force_inline __forceinline
#endif
#endif

// #ifdef __GNUC__
// #ifndef default_inline
// #define default_inline inline
// #define force_inline inline
// #endif
// #endif

#ifndef default_inline
#define default_inline
#define force_inline
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#ifndef offsetof
#define offsetof(Ty, Fi)	((nlint)(&(((Ty *)0)->Fi)))
#endif

typedef struct BJX2_FILE_s *BJX2_FILE;

#ifdef _MSC_VER
#define JX2_DBGBREAK	__debugbreak();
#else
#ifdef __GNUC__
#define JX2_DBGBREAK	__builtin_trap();
#else
// #define BGBCC_DBGBREAK
#endif
#endif

// #ifdef _WIN32
// #define JX2_DBGBREAK	__debugbreak();
// #else
// #define JX2_DBGBREAK
// #endif

#ifndef JX2_DBGBREAK
#define JX2_DBGBREAK
#endif

#include "jx2i_misal.h"

typedef struct BJX2_Context_s BJX2_Context;
typedef struct BJX2_Opcode_s BJX2_Opcode;
typedef struct BJX2_Trace_s BJX2_Trace;
typedef struct BJX2_MemSpan_s BJX2_MemSpan;

struct BJX2_Context_s {
u64 regs[128];				//GPRs and CRs
// u64 fpreg[32];				//FPRs

u64 ex_regs[128];				//GPRs and CRs
// u64 ex_fpreg[32];				//FPRs

BJX2_Trace *rttr[64];		//return traces (mini hash)
BJX2_Trace *prttr;			//return traces (pred)

// BJX2_Trace *trhash[1024];
BJX2_Trace *trhash[4096];
BJX2_Trace *trcur;			//cached trace for interpreter loop.

BJX2_MemSpan *span[64];	//memory spans, sorted by address
int n_span;

bjx2_addr	 psp_pbase;		//Predict Span/Page Base
bjx2_addr	 psp_prng3;		//Predict Span/Page Range-3
byte		*psp_pdata;		//Predict Span/Page Data

BJX2_Context	*ctx_parent;
BJX2_Context	*ctx_child;
BJX2_Context	*ctx_next;

BJX2_MemSpan *span_pr0;
BJX2_MemSpan *span_pr1;

bjx2_addr	ppa_addr[64];		//page-predict addr
bjx2_addr	ppa_addrh[64];		//page-predict addr (high)
byte		*ppa_data[64];

BJX2_Opcode *free_op;
BJX2_Trace *free_tr;

BJX2_Trace *tr_cur;		//Current Trace (Runtime)
BJX2_Trace *tr_rnxt;		//Trace lnknext (Runtime)
BJX2_Trace *tr_rjmp;		//Trace jmpnext (Runtime)

u32 pclog[64];
byte pclogrov;

byte no_memcost;
byte use_jit;
byte wexmd;
byte v_wexmd;
byte cc_flush;
byte core_id;

int status;
bjx2_addr trapc;

byte req_kill;				//VM has been requested to stop
byte kbbuf[256];			//debug keyboard buffer
byte kbrov;					//debug keyboard rover
byte kbirov;				//debug keyboard rover
byte kbirq;					//keyboard IRQ

byte ps2kbbuf[256];			//debug keyboard buffer
byte ps2kbrov;					//debug keyboard rover
byte ps2kbirov;				//debug keyboard rover

byte ps2msbuf[256];			//debug keyboard buffer
byte ps2msrov;					//debug keyboard rover
byte ps2msirov;				//debug keyboard rover

int ttick_hk;				//timer ticks until IRQ
int ttick_rst;				//timer ticks reset
s64 tot_cyc;
s64 tot_cyc_ip;				//interlock penalty
s64 tot_cyc_mem;
s64 tot_cyc_miss;
s64 tot_cyc_miss_l1;
s64 tot_cyc_miss_l1i;
s64 tot_cyc_miss_l2;
s64 tot_cyc_miss_l2i;
s64 tot_cyc_miss_l2v;
s64 tot_cyc_miss_bra;
s64 tot_cyc_mmio;
s64 tot_ops;
s64 tot_nbops;
s16 tgt_mhz;				//target MHz
u16 rcp_mhz;				//reciprocal MHz
int iodel_cyc;				//IO delay cycles

byte	use_walltime;

int nttick_irq;				//number of timer-tick IRQs
int mem_cyc;				//cache miss cycles
int miss_cyc;				//cache miss cycles
int miss_cyc_l1;			//cache miss cycles (L1 miss)
int miss_cyc_l1i;			//cache miss cycles (L1 miss)
int miss_cyc_l2;			//cache miss cycles (L2 miss)
int miss_cyc_l2i;			//cache miss cycles (L2 miss)
int mem_cyc_mmio;			//cycles spent in MMIO

int miss_cyc_bra;			//branch miss cycles

int mem_cnt_mem;			//cache miss cycles
int mem_cnt_l1;				//cache miss cycles
int mem_cnt_l1i;			//cache miss cycles
int mem_cnt_l2;				//cache miss cycles
int mem_cnt_l2i;			//cache miss cycles
int mem_cnt_dram;			//cache miss cycles
int mem_cnt_vihit;			//victim hit

int miss_cnt_l1;			//cache miss cycles (L1 miss D$)
int miss_cnt_l1i;			//cache miss cycles (L1 miss I$)
int miss_cnt_l2;			//cache miss cycles (L2 miss)
int miss_cnt_l2i;			//cache miss cycles (L2 miss)
int mem_cnt_dri;			//cache miss cycles
int mem_cnt_drd;			//cache miss cycles

s64 tot_cnt_mem;
s64 tot_cnt_mem_l1;
s64 tot_cnt_mem_l1i;
s64 tot_cnt_mem_l2;
s64 tot_cnt_mem_l2i;
s64 tot_cnt_mem_l2v;
s64 tot_cnt_mem_l2vihit;
s64 tot_cnt_mem_dram;
s64 tot_cnt_miss_l1;
s64 tot_cnt_miss_l2;
s64 tot_cnt_miss_l2i;
s64 tot_cnt_miss_l2v;
s64 tot_cnt_miss_l1i;
int tot_cnt_mem_dri;		//total I$ to DRAM misses
int tot_cnt_mem_drd;		//total D$ to DRAM misses

s64 tot_cnt_tlbmiss;

byte mem_l2rov;
byte mem_l1rov;
byte mem_vidrov;

byte bpr_tab[256];			//state tables
byte bpr_sctab[256];		//state tables
s64 bpr_hit;
s64 bpr_cnt;
int bpr_hist;

byte *bpr_gatab[256];		//state tables for GA
byte *bpr_gatab_buf;

byte *bpr_ga_adjt[256];		//true-state update table
byte *bpr_ga_adjf[256];		//false-state update table
u16 bpr_ga_hit[256];		//true-state update table
byte *bpr_ga_adj_buf;
byte bpr_ga_rov;
int bpr_ga_cnt;
int bpr_ga_gencnt;

bjx2_addr mem_l1addr1;		//L1 addr
bjx2_addr mem_l1addr2;		//L1 addr
bjx2_addr mem_l1addr3;		//L1 addr
bjx2_addr mem_l1addr4;		//L1 addr

bjx2_addr mem_l2addr1;		//L2 addr
bjx2_addr mem_l2addr2;		//L2 addr
bjx2_addr mem_l2addr3;		//L2 addr
bjx2_addr mem_l2addr4;		//L2 addr

u64		hw_rng[4];

u64		krr_key[2];
int		vsk_rng;

// bjx2_addr	mem_l1m[16][16384];	//L1 addr (Multi)
bjx2_addr	*mem_l1m[64];
s64			mem_l1m_miss[64];
s64			mem_l1m_cmiss[64];
s64			mem_l1m_umiss[64];
s64			mem_l1m_cnt;

bjx2_addr mem_l1h4k[8192];		//L1 addr (4kB)
bjx2_addr mem_l1ih4k[8192];		//L1 addr (4kB)

bjx2_addr mem_l1ht[16];			//L1 tiny

// bjx2_addr mem_l2h32k[8192];	//L2 addr (32/64kB)
bjx2_addr mem_l2h32k[16384];	//L2 addr (32/64kB)


short	l1i_hmask;
short	l1d_hmask;
byte	l1i_wmask;
byte	l1d_wmask;

short	l2_hmask;
byte	l2_wmask;
byte	l2_hshr;
byte	l2_vict;		//L2 enable victim buffer

// u64 mem_tlb_hi[64*4];
// u64 mem_tlb_lo[64*4];
u64 mem_tlb_hi[256*4];
u64 mem_tlb_lo[256*4];

u64 mem_tlb_acl[4];

u64 mem_tlb_pr0_hi;
u64 mem_tlb_pr0_hx;
u64 mem_tlb_pr0_lo;
u64 mem_tlb_pr1_hi;
u64 mem_tlb_pr1_hx;
u64 mem_tlb_pr1_lo;

u64 mem_ldtlb_hix;
u64 mem_ldtlb_lox;

char *map_iname[256];
bjx2_addr *map_addr[256];
char **map_name[256];
u16 *map_mode[256];
int map_n_ents[256];
bjx2_addr map_addr_min[256];
bjx2_addr map_addr_max[256];

int map_b_lln[256];
int map_n_lln[256];
char *map_pbase[256];

int n_map;

bjx2_addr dbg_setbrk_pc[256];
byte dbg_setbrk_rov;
byte dbg_setbrk_nz;


bjx2_addr	dbg_data_start;
bjx2_addr	dbg_data_end;
bjx2_addr	dbg_bss_start;
bjx2_addr	dbg_bss_end;

int ms0, lms1;
u32 rtc_ms;

u32 *msgbuf_rx;
u32 *msgbuf_tx;
u32 msgbuf_msk;
u32 msgbuf_rxspos;
u32 msgbuf_rxepos;
u32 msgbuf_txspos;
u32 msgbuf_txepos;

byte usbkb_report[8];
byte do_usb_hid;

u64 *usbbuf;
u64 *usbbuf_rxa;
u64 *usbbuf_txa;
u64 *usbbuf_rxb;
u64 *usbbuf_txb;

u32 usbbuf_rxsposa;
u32 usbbuf_rxeposa;
u32 usbbuf_txsposa;
u32 usbbuf_txeposa;

u32 usbbuf_rxsposb;
u32 usbbuf_rxeposb;
u32 usbbuf_txsposb;
u32 usbbuf_txeposb;

char puts_linebuf[256];
byte puts_linepos;

char		*map_img_name[256];
bjx2_addr	map_img_base[256];
byte		n_map_img;

BJX2_MemSpan *(*MemSpanForAddr)(BJX2_Context *ctx, bjx2_addr addr);

#if 0
int (*MemGetByte)(BJX2_Context *ctx, bjx2_addr addr0);
int (*MemGetWord)(BJX2_Context *ctx, bjx2_addr addr0);
s32 (*MemGetDWord)(BJX2_Context *ctx, bjx2_addr addr0);
s64 (*MemGetQWord)(BJX2_Context *ctx, bjx2_addr addr0);
int (*MemGetXWord)(BJX2_Context *ctx, bjx2_addr addr0, u64 *rvlo, u64 *rvhi);

int (*MemSetByte)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemSetWord)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemSetDWord)(BJX2_Context *ctx, bjx2_addr addr0, s32 val);
int (*MemSetQWord)(BJX2_Context *ctx, bjx2_addr addr0, s64 val);
int (*MemSetXWord)(BJX2_Context *ctx, bjx2_addr addr0, u64 vlo, u64 vho);
int (*MemSetTripwire)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemQueryTransit)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addr1, int val);
#endif

#if 1
int (*MemGetByte)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh);
int (*MemGetWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh);
s32 (*MemGetDWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh);
s64 (*MemGetQWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh);
int (*MemGetXWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, u64 *rvlo, u64 *rvhi);

int (*MemSetByte)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, int val);
int (*MemSetWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, int val);
int (*MemSetDWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, s32 val);
int (*MemSetQWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, s64 val);
int (*MemSetXWord)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, u64 vlo, u64 vho);
int (*MemSetTripwire)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addrh, int val);

int (*MemQueryTransit)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addr1, bjx2_addr addrh, int val);
#endif

};

struct BJX2_Opcode_s {
u16 opn;		//Opcode Value
u16 opn2;		//Opcode Value
u16 opn3;		//Opcode Value
u16 nmid;		//Opcode Number

byte fmid;		//Form ID
byte rn;		//Dest Register
byte rm;		//Source Register
byte ro;		//Source Register

byte rp;		//Source Register (4R)
byte rq;		//Quadrant Register
byte ldsc;		//Load Scale
byte ldop;		//Load Operation

s16 cyc;		//Clock Cycles
u32 fl;			//Opcode Flags
// s32 imm;		//Immediate
s64 imm;		//Immediate

bjx2_addr pc;
bjx2_addr pc2;

void (*Run)(BJX2_Context *ctx, BJX2_Opcode *op);
void *data;
};

struct BJX2_Trace_s {
// BJX2_Opcode *ops[BJX2_TR_MAXOP+2];
BJX2_Opcode *ops[BJX2_TR_MAXOP];
sbyte n_ops;
sbyte n_nbops;
s16 n_cyc;				//Baseline Cycles
s16 ip_cyc;				//Interlock Penalty Cycles
s16	jit_inh;
u16 addr_hsr;			//hash of status registers
s64 runcnt;
s64 acc_pencyc;			//accumulated penalty cycles

bjx2_addr addr;
bjx2_addr addr_nxt;
bjx2_addr addr_jmp;
BJX2_Trace *hnext;		//next in terms of hash
BJX2_Trace *lnknext;	//next in terms of PC
BJX2_Trace *jmpnext;	//next if ending in a branch

BJX2_Trace *(*Run)(BJX2_Context *ctx, BJX2_Trace *tr);
};

struct BJX2_MemSpan_s {
bjx2_addr addr_base;	//lower address
bjx2_addr addr_lim;		//upper address
bjx2_addru addr_sz;		//address-space size

char *name;				//helpful name
void *data;				//raw data pointer
// byte *tripwire;			//tripwire mask
u32 *tripwire;			//tripwire mask
byte simple_mem;		//simple memory read
bjx2_addr modbase;		//modulo base
bjx2_addr modmask;		//modulo mask

int (*GetByte)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr);
int (*GetWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr);
s32 (*GetDWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr);
s64 (*GetQWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr);
int (*GetXWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 *rvlo, u64 *rvhi);

int (*SetByte)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val);
int (*SetWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val);
int (*SetDWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val);
int (*SetQWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val);
int (*SetXWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 vlo, u64 vhi);
int (*SetTripwire)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int mode);
int (*MemQueryTransit)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr0, bjx2_addr addr1, int val);
};

BJX2_Opcode *BJX2_ContextAllocOpcode(BJX2_Context *ctx);
BJX2_Trace *BJX2_ContextAllocTrace(BJX2_Context *ctx);
BJX2_Trace *BJX2_GetTraceForAddr(BJX2_Context *ctx, bjx2_addr addr, int tfl);
int BJX2_ThrowFaultStatus(BJX2_Context *ctx, int status);
int BJX2_FaultLeaveInterrupt(BJX2_Context *ctx);


BJX2_MemSpan *BJX2_MemAllocSpan(BJX2_Context *ctx);
int BJX2_MemAddSpan(BJX2_Context *ctx, BJX2_MemSpan *sp);

BJX2_FILE *bjx2_fopen(char *name, char *mode);
int bjx2_fclose(BJX2_FILE *fd);
int bjx2_feof(BJX2_FILE *fd);
s64 bjx2_ftell(BJX2_FILE *fd);
int bjx2_fseek(BJX2_FILE *fd, s64 pos, int md);
int bjx2_fread(void *buf, int n, int m, BJX2_FILE *fd);
int bjx2_fwrite(void *buf, int n, int m, BJX2_FILE *fd);
int bjx2_fflush(BJX2_FILE *fd);
int bjx2_vmputc(BJX2_Context *ctx, int val);

int BJX2_VmMsgRxUpdate(BJX2_Context *ctx);
int BJX2_VmMsgTxUpdate(BJX2_Context *ctx);

int BJX2_MemRamCb_GetFaultXW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 *rvlo, u64 *rvhi);
int BJX2_MemRamCb_SetFaultXW(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, u64 vlo, u64 vhi);



BJX2_MemSpan *BJX2_MemSpanForName(BJX2_Context *ctx, char *name);
BJX2_MemSpan *BJX2_MemSpanForAddr(BJX2_Context *ctx, bjx2_addr addr);
