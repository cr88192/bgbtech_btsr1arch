/*
 Copyright (c) 2018-2020 Brendan G Bohannon

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

// #define BJX2_REG_REMAP

#ifndef BJX2_REG_REMAP
#define BJX2_REG_DLR	0
#define BJX2_REG_DHR	1
#define BJX2_REG_SP		15
#endif

#ifdef BJX2_REG_REMAP
#define BJX2_REG_DLR	32
#define BJX2_REG_DHR	33
#define BJX2_REG_SP		47
#endif

#define BJX2_REG_DR		BJX2_REG_DLR

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
#define BJX2_REG_IMM	78
#define BJX2_REG_ZZR	79
#endif


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

#define BJX2_TR_MAXOP	32


#define BJX2_FLT_INVADDR	0x8000		//invalid memory address
#define BJX2_FLT_INV_MRD	0x8001		//invalid memory read
#define BJX2_FLT_INV_MWR	0x8002		//invalid memory write
#define BJX2_FLT_INV_MEX	0x8003		//invalid memory execute
#define BJX2_FLT_BREAK		0x8004		//BREAK
#define BJX2_FLT_SLEEP		0x8005		//SLEEP
#define BJX2_FLT_INVOP		0x8006		//Invalid Opcode
#define BJX2_FLT_MISAL		0x8007		//Invalid Misaligned Access
#define BJX2_FLT_BOUNDCHK	0x8008		//Bounds Check Fail

#define BJX2_FLT_PCMISH		0x8801		//PC doesn't match trace addr
#define BJX2_FLT_CCFLUSH	0x8802		//Cache Flush
#define BJX2_FLT_BADPC		0x8803		//Invalid PC

#define BJX2_FLT_TLBGF		0xA000		//TLB General Fault
#define BJX2_FLT_TLBMISS	0xA001		//TLB Miss
#define BJX2_FLT_TLBACL		0xA002		//TLB ACL

#define BJX2_FLT_TIMER		0xC001		//kHz timer
#define BJX2_FLT_IOPOKE		0xC002		//IO Poke
#define BJX2_FLT_SCRPOKE	0xC003		//Screen Poke

#define BJX2_OPFL_CTRLF		0x01		//Control-Flow Opcode
#define BJX2_OPFL_TWOWORD	0x02		//Uses two instruction words
#define BJX2_OPFL_TRIWORD	0x04		//Uses three instruction words
#define BJX2_OPFL_WEX		0x08		//Wide-Executed Op
#define BJX2_OPFL_JUMBO64	0x10		//Uses four instruction words
#define BJX2_OPFL_JUMBO96	0x20		//Uses six instruction words
#define BJX2_OPFL_NOWEX		0x40		//Invalid as a WEX form
#define BJX2_OPFL_NOWEXSFX	0x80		//Invalid as a WEX suffix

#define BJX2_OPFL_REGXM		0x0100		//Rm is Xm
#define BJX2_OPFL_REGXN		0x0200		//Rn is Xn
#define BJX2_OPFL_REGXO		0x0400		//Ro is Xo
#define BJX2_OPFL_REGX2R	0x0300		//Rm and Rn are Xn
#define BJX2_OPFL_REGX3R	0x0700		//Rm, Rn, and Ro are Xn
#define BJX2_OPFL_OP24		0x0800		//24-bit instruction

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
#define BJX2_NMID_FMOVD		0x4F		//

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
#define BJX2_NMID_ROTLX			0xE8		//
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

#define BJX2_NMID_PLDCH			0xF4		//
#define BJX2_NMID_PLDCHH		0xF5		//
#define BJX2_NMID_PSTCH			0xF6		//
#define BJX2_NMID_PSTCHH		0xF7		//
#define BJX2_NMID_PLDCHX		0xF8		//
#define BJX2_NMID_PSTCHX		0xF9		//
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

#define BJX2_FLIPSTNM

// #define BJX2_EM_BPRED

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
u64 fpreg[32];				//FPRs

u64 ex_regs[128];				//GPRs and CRs
u64 ex_fpreg[32];				//FPRs

BJX2_Trace *rttr[64];		//return traces (mini hash)
BJX2_Trace *prttr;			//return traces (pred)

BJX2_Trace *trhash[1024];
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
s64 tot_cyc_mem;
s64 tot_cyc_miss;
s64 tot_cyc_miss_l1;
s64 tot_cyc_miss_l2;
s64 tot_cyc_mmio;
s64 tot_ops;
s16 tgt_mhz;				//target MHz
u16 rcp_mhz;				//reciprocal MHz
int iodel_cyc;				//IO delay cycles

int nttick_irq;				//number of timer-tick IRQs
int mem_cyc;				//cache miss cycles
int miss_cyc;				//cache miss cycles
int miss_cyc_l1;			//cache miss cycles (L1 miss)
int miss_cyc_l2;			//cache miss cycles (L2 miss)
int mem_cyc_mmio;			//cycles spent in MMIO

int mem_cnt_l1;				//cache miss cycles
int mem_cnt_l2;				//cache miss cycles
int mem_cnt_l1i;			//cache miss cycles
int miss_cnt_l1;			//cache miss cycles (L1 miss D$)
int miss_cnt_l2;			//cache miss cycles (L2 miss)
int miss_cnt_l1i;			//cache miss cycles (L1 miss I$)
int mem_cnt_dri;			//cache miss cycles
int mem_cnt_drd;			//cache miss cycles

s64 tot_cnt_mem_l1;
s64 tot_cnt_mem_l2;
s64 tot_cnt_mem_l1i;
s64 tot_cnt_miss_l1;
s64 tot_cnt_miss_l2;
s64 tot_cnt_miss_l1i;
int tot_cnt_mem_dri;		//total I$ to DRAM misses
int tot_cnt_mem_drd;		//total D$ to DRAM misses

s64 tot_cnt_tlbmiss;

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

bjx2_addr mem_l1h4k[4096];		//L1 addr (4kB)
// bjx2_addr mem_l2h32k[8192];	//L2 addr (32/64kB)
bjx2_addr mem_l2h32k[16384];	//L2 addr (32/64kB)

bjx2_addr mem_l1ih4k[4096];		//L1 addr (4kB)

// u64 mem_tlb_hi[64*4];
// u64 mem_tlb_lo[64*4];
u64 mem_tlb_hi[256*4];
u64 mem_tlb_lo[256*4];

u64 mem_tlb_pr0_hi;
u64 mem_tlb_pr0_lo;
u64 mem_tlb_pr1_hi;
u64 mem_tlb_pr1_lo;

bjx2_addr *map_addr;
char **map_name;
int map_n_ents;

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

BJX2_MemSpan *(*MemSpanForAddr)(BJX2_Context *ctx, bjx2_addr addr);

int (*MemGetByte)(BJX2_Context *ctx, bjx2_addr addr0);
int (*MemGetWord)(BJX2_Context *ctx, bjx2_addr addr0);
s32 (*MemGetDWord)(BJX2_Context *ctx, bjx2_addr addr0);
s64 (*MemGetQWord)(BJX2_Context *ctx, bjx2_addr addr0);

int (*MemSetByte)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemSetWord)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemSetDWord)(BJX2_Context *ctx, bjx2_addr addr0, s32 val);
int (*MemSetQWord)(BJX2_Context *ctx, bjx2_addr addr0, s64 val);
int (*MemSetTripwire)(BJX2_Context *ctx, bjx2_addr addr0, int val);
int (*MemQueryTransit)(BJX2_Context *ctx,
	bjx2_addr addr0, bjx2_addr addr1, int val);
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
sbyte cyc;		//Clock Cycles
u16 fl;			//Opcode Flags
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
int n_ops;
int n_cyc;
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

int (*SetByte)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val);
int (*SetWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val);
int (*SetDWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val);
int (*SetQWord)(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s64 val);
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



BJX2_MemSpan *BJX2_MemSpanForAddr(BJX2_Context *ctx, bjx2_addr addr);
